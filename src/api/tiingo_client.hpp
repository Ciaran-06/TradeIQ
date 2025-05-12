#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "core/price_series.hpp"
#include "api/http_client.hpp" 
#include "api/default_http_client.hpp"

class TiingoClient {
public:
    TiingoClient(); // default
    TiingoClient(const std::string& key, std::shared_ptr<HttpClient> client); // ✅ this is the missing one

    void setOfflineMode(bool flag);
    void setVerbosity(bool verbose);

    PriceSeries fetchDailyPrices(const std::string& ticker,
                                 const std::string& startDate,
                                 const std::string& endDate,
                                 const std::string& frequency = "daily");

    std::map<std::string, PriceSeries> fetchMultipleDailyPrices(const std::vector<std::string>& tickers,
                                                                 const std::string& startDate,
                                                                 const std::string& endDate,
                                                                 const std::string& frequency = "daily");

private:
    std::string apiKey_;
    bool offlineMode_ = false;
    bool verbose_ = false;

    std::shared_ptr<HttpClient> http_; // ✅ make sure this is declared

    std::string buildUrl(const std::string& ticker,
                         const std::string& startDate,
                         const std::string& endDate,
                         const std::string& frequency) const;

    void validateInputs(const std::string& ticker,
                        const std::string& startDate,
                        const std::string& endDate) const;

    PriceSeries parseResponse(const std::string& ticker, const std::string& responseBody);
    std::string tryCachedResponse(const std::string& ticker, const std::string& startDate, const std::string& endDate);
    void cacheResponse(const std::string& ticker, const std::string& startDate, const std::string& endDate, const std::string& body);
};
