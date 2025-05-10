#pragma once

#include <string>
#include <vector>

class PriceSeries {
public:
    PriceSeries(const std::string& ticker,
                const std::vector<std::string>& dates,
                const std::vector<double>& prices);

    const std::string& getTicker() const;
    const std::vector<std::string>& getDates() const;
    const std::vector<double>& getPrices() const;

    std::vector<double> getDailyReturns() const;

private:
    std::string ticker_;
    std::vector<std::string> dates_;
    std::vector<double> prices_;
};
