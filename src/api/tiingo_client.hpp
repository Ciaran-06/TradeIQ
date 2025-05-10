// src/api/tiingo_client.hpp
#pragma once

#include <string>
#include <map>
#include <vector>
#include "../core/price_series.hpp"

class TiingoClient
{
public:
    TiingoClient();

    // Fetch daily prices for a single ticker
    PriceSeries fetchDailyPrices(const std::string &ticker,
                                 const std::string &startDate,
                                 const std::string &endDate,
                                 const std::string &frequency = "daily");

    // Fetch prices for multiple tickers
    std::map<std::string, PriceSeries> fetchMultipleDailyPrices(const std::vector<std::string> &tickers,
                                                                const std::string &startDate,
                                                                const std::string &endDate,
                                                                const std::string &frequency = "daily");

    void setOfflineMode(bool flag);
    void setVerbosity(bool verbose);

private:
    std::string apiKey_;
    bool offlineMode_ = false;
    bool verbose_ = false;

    std::string buildUrl(const std::string &ticker,
                         const std::string &startDate,
                         const std::string &endDate,
                         const std::string &frequency) const;
    PriceSeries parseResponse(const std::string &ticker, const std::string &responseBody);
    void validateInputs(const std::string &ticker, const std::string &startDate, const std::string &endDate) const;
    std::string tryCachedResponse(const std::string &ticker, const std::string &startDate, const std::string &endDate);
    void cacheResponse(const std::string &ticker, const std::string &startDate, const std::string &endDate, const std::string &body);
};
