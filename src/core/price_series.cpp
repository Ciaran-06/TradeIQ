#include "price_series.hpp"
#include <stdexcept>

PriceSeries::PriceSeries(const std::string& ticker,
                         const std::vector<std::string>& dates,
                         const std::vector<double>& prices)
    : ticker_(ticker), dates_(dates), prices_(prices) {
    if (dates_.size() != prices_.size()) {
        throw std::invalid_argument("Dates and prices must have the same length");
    }
}

const std::string& PriceSeries::getTicker() const {
    return ticker_;
}

const std::vector<std::string>& PriceSeries::getDates() const {
    return dates_;
}

const std::vector<double>& PriceSeries::getPrices() const {
    return prices_;
}

std::vector<double> PriceSeries::getDailyReturns() const {
    std::vector<double> returns;
    if (prices_.size() < 2) return returns;

    for (size_t i = 1; i < prices_.size(); ++i) {
        double ret = (prices_[i] - prices_[i - 1]) / prices_[i - 1];
        returns.push_back(ret);
    }

    return returns;
}
