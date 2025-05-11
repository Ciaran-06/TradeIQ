#pragma once

#include <vector>
#include "price_series.hpp"

class StatsEngine
{
public:
    static std::vector<double> computeDailyReturns(const PriceSeries &series);
    static std::vector<double> meanReturns(const std::vector<PriceSeries> &seriesList);
    static std::vector<std::vector<double>> covarianceMatrix(const std::vector<PriceSeries> &seriesList);
    static double portfolioVariance(
        const std::vector<std::vector<double>> &covMatrix,
        const std::vector<double> &weights);
    static double expectedPortfolioReturn(const std::vector<double> &meanReturns, const std::vector<double> &weights);
    static double computeSharpeRatio(double expectedPortfolioReturn, double portfolioVariance, double riskFreeRate);
    static double computeSortinoRatio(double expectedPortfolioReturn, double riskFreeReturn, const std::vector<double> &portfolioReturns);
    static double computeMaxDrawdown(const std::vector<double> &cumulativeReturns);
    static std::vector<double> computeRollingVolatility(const std::vector<double> &returns, int windowSize);
    static std::vector<double> computeRollingSharpe(const std::vector<double>& returns, int windowSize, double riskFreeRate); 
};