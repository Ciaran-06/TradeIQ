#pragma once

#include <string>
#include <vector>

class PriceSeries
{
public:
    // Existing full constructor
    PriceSeries(const std::string &ticker,
                const std::vector<std::string> &dates,
                const std::vector<double> &prices);

    // ➕ New constructor for tests (assumes dummy dates)
    PriceSeries(const std::string &ticker,
                const std::vector<double> &prices)
        : ticker_(ticker), prices_(prices)
    {
        dates_.resize(prices.size(), ""); // fill with empty strings
    }

    const std::string &getTicker() const;
    const std::vector<std::string> &getDates() const;
    const std::vector<double> &getPrices() const;
    std::vector<double> getDailyReturns() const;

private:
    std::string ticker_;
    std::vector<std::string> dates_;
    std::vector<double> prices_;
};
