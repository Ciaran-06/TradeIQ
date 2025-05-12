#include <fstream>
#include <filesystem>
#include <regex>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <memory>

#include <cpr/cpr.h>
#include "../external/dotenv.h"
#include "../external/json.hpp"
#include "api/tiingo_client.hpp"
#include "api/http_client.hpp"
#include "api/default_http_client.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

bool isValidDate(const std::string &dateStr)
{
    if (dateStr.size() != 10 || dateStr[4] != '-' || dateStr[7] != '-')
        return false;
    try
    {
        int year = std::stoi(dateStr.substr(0, 4));
        int month = std::stoi(dateStr.substr(5, 2));
        int day = std::stoi(dateStr.substr(8, 2));
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > 31)
            return false;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

TiingoClient::TiingoClient()
{
    fs::path envPath = fs::current_path() / ".env";
    if (!fs::exists(envPath))
    {
        // Try project root fallback (if running from build/ or bin/)
        envPath = fs::current_path().parent_path() / ".env";
    }

    if (!fs::exists(envPath))
    {
        throw std::runtime_error(".env file not found.");
    }

    dotenv::init(envPath.string().c_str());

    const char *key = std::getenv("API_KEY");
    if (!key)
        throw std::runtime_error("API_KEY not found in .env");
    apiKey_ = key;
    http_ = std::make_shared<DefaultHttpClient>();
}

TiingoClient::TiingoClient(const std::string &key, std::shared_ptr<HttpClient> client)
    : apiKey_(key), http_(client ? client : std::make_shared<DefaultHttpClient>()) {}

void TiingoClient::setOfflineMode(bool flag) { offlineMode_ = flag; }
void TiingoClient::setVerbosity(bool verbose) { verbose_ = verbose; }

PriceSeries TiingoClient::fetchDailyPrices(const std::string &ticker,
                                           const std::string &startDate,
                                           const std::string &endDate,
                                           const std::string &frequency)
{
    std::string cached = tryCachedResponse(ticker, startDate, endDate);
    if (!cached.empty())
    {
        if (verbose_)
            std::cout << "[Cache hit] " << ticker << std::endl;
        return parseResponse(ticker, cached);
    }

    if (offlineMode_)
    {
        throw std::runtime_error("Offline mode enabled and no cache found.");
    }

    validateInputs(ticker, startDate, endDate);
    std::string url = buildUrl(ticker, startDate, endDate, frequency);

    if (verbose_)
        std::cout << "[Fetching] " << url << std::endl;

    std::string responseBody;
    int retries = 3;
    while (retries-- > 0)
    {
        try
        {
            responseBody = http_->get(url);
            break;
        }
        catch (const std::runtime_error &e)
        {
            if (retries == 0)
                throw;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    cacheResponse(ticker, startDate, endDate, responseBody);
    return parseResponse(ticker, responseBody);
}

std::map<std::string, PriceSeries> TiingoClient::fetchMultipleDailyPrices(const std::vector<std::string> &tickers,
                                                                          const std::string &startDate,
                                                                          const std::string &endDate,
                                                                          const std::string &frequency)
{
    std::map<std::string, PriceSeries> result;
    for (const auto &ticker : tickers)
    {
        try
        {
            auto ps = fetchDailyPrices(ticker, startDate, endDate, frequency);
            result.insert({ticker, ps});
        }
        catch (const std::exception &e)
        {
            if (verbose_)
                std::cerr << "[Error] " << ticker << ": " << e.what() << std::endl;
        }
    }
    return result;
}

std::string TiingoClient::buildUrl(const std::string &ticker,
                                   const std::string &startDate,
                                   const std::string &endDate,
                                   const std::string &frequency) const
{
    if (frequency == "daily")
    {
        return "https://api.tiingo.com/tiingo/daily/" + ticker + "/prices?startDate=" + startDate +
               "&endDate=" + endDate + "&token=" + apiKey_;
    }
    else if (frequency == "intraday")
    {
        return "https://api.tiingo.com/iex/" + ticker + "?startDate=" + startDate +
               "&endDate=" + endDate + "&resampleFreq=5min&columns=open,close&token=" + apiKey_;
    }
    else
    {
        throw std::invalid_argument("Unsupported frequency: " + frequency);
    }
}

void TiingoClient::validateInputs(const std::string &ticker,
                                  const std::string &startDate,
                                  const std::string &endDate) const
{
    if (ticker.empty() || startDate.empty() || endDate.empty())
    {
        throw std::invalid_argument("Ticker and dates must not be empty.");
    }

    if (!isValidDate(startDate) || !isValidDate(endDate))
    {
        throw std::invalid_argument("Dates must be valid and in YYYY-MM-DD format.");
    }
}

PriceSeries TiingoClient::parseResponse(const std::string &ticker, const std::string &responseBody)
{
    json data = json::parse(responseBody);
    if (!data.is_array() || data.empty())
    {
        throw std::runtime_error("Tiingo returned no data.");
    }

    std::vector<std::string> dates;
    std::vector<double> prices;

    for (const auto &row : data)
    {
        if (!row.contains("date") || !row.contains("adjClose"))
            continue;
        dates.push_back(row["date"]);
        prices.push_back(row["adjClose"]);
    }

    return PriceSeries(ticker, dates, prices);
}

std::string TiingoClient::tryCachedResponse(const std::string &ticker, const std::string &startDate, const std::string &endDate)
{
    std::string cachePath = ".cache/" + ticker + "_" + startDate + "_" + endDate + ".json";
    if (fs::exists(cachePath))
    {
        std::ifstream file(cachePath);
        return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    }
    return "";
}

void TiingoClient::cacheResponse(const std::string &ticker, const std::string &startDate, const std::string &endDate, const std::string &body)
{
    fs::create_directories(".cache");
    std::string cachePath = ".cache/" + ticker + "_" + startDate + "_" + endDate + ".json";
    std::ofstream out(cachePath);
    out << body;
}
