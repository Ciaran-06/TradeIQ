#include "stats/returns.hpp"
#include <numeric>
#include <stdexcept>
#include <cmath>

namespace Stats::Returns {

std::vector<double> computeDailyReturns(const PriceSeries &series) {
    const auto& prices = series.getPrices();

    if (prices.size() < 2) {
        throw std::invalid_argument("At least two prices are required to compute daily returns.");
    }

    std::vector<double> returns;
    returns.reserve(prices.size() - 1);

    for (size_t i = 1; i < prices.size(); ++i) {
        double prev = prices[i - 1];
        double curr = prices[i];

        if (prev == 0.0) {
            throw std::invalid_argument("Encountered zero price, cannot compute return.");
        }

        returns.push_back((curr - prev) / prev);
    }

    return returns;
}


double meanReturns(const std::vector<double> &returns) {
    if (returns.empty()) throw std::invalid_argument("Returns vector is empty");
    double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
    return sum / static_cast<double>(returns.size());
}

double computeTotalReturn(const PriceSeries& series) {
    const auto& prices = series.getPrices();

    if (prices.size() < 2) {
        throw std::invalid_argument("At least two prices are required to compute total return.");
    }


    double start = prices.front();
    double end = prices.back();

    if(start == 0) throw std::invalid_argument("Start price cannot be 0");
    return (end - start) / start;
}

double computeAnnualizedReturn(double totalReturn, int numPeriods, int periodsPerYear) {
    if (numPeriods <= 0 || periodsPerYear <= 0)
        throw std::invalid_argument("Invalid period input for annualized return");

    return std::pow(1.0 + totalReturn, static_cast<double>(periodsPerYear) / numPeriods) - 1.0;
}

double expectedPortfolioReturn(const std::vector<double> &meanReturns,
                               const std::vector<double> &weights) {
    if (meanReturns.size() != weights.size())
        throw std::invalid_argument("Mismatched lengths in expectedPortfolioReturn");

    double result = 0.0;
    for (size_t i = 0; i < meanReturns.size(); ++i) {
        result += meanReturns[i] * weights[i];
    }
    return result;
}

}
