#pragma once

#include <vector>

namespace Stats::Volatility {

    double computeAnnualizedVolatility(const std::vector<double>& returns, int periodsPerYear);

    std::vector<double> computeRollingVolatility(const std::vector<double>& returns, size_t window);

    std::vector<double> computeRollingStandardDeviation(const std::vector<double>& returns, bool sample = false);

    std::vector<double> computeRollingSharpe(const std::vector<double>& returns, int windowSize, double riskFreeRate);

}