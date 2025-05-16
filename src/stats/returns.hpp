/*
computeDailyReturns
meanReturns
computeTotalReturn
computeAnnualizedReturn
expectedPortfolioReturn
*/

#pragma once

#include "core/price_series.hpp"
#include <vector>
#include <string>

namespace Stats::Returns {

    // Compute simple daily percentage returns from price series
    std::vector<double> computeDailyReturns(const PriceSeries &series);

    // Mean of any return vector
    double meanReturns(const std::vector<double> &returns);

    // Cumulative total return from price series
    double computeTotalReturn(const PriceSeries &series);

    // Annualized return from total return, periods elapsed, and periods per year
    double computeAnnualizedReturn(double totalReturn, int numPeriods, int periodsPerYear);

    // Weighted expected return from mean return vector and weights
    double expectedPortfolioReturn(const std::vector<double> &meanReturns,
                                   const std::vector<double> &weights);

}
