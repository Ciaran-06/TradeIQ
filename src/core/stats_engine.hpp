#pragma once

#include "price_series.hpp"
#include <vector>
#include <string>
#include <map>
#include <numeric>
#include "math_utils.hpp"

class StatsEngine
{
public:
    static std::vector<double> computeDailyReturns(const PriceSeries &);
    static double meanReturns(const std::vector<double> &);
    static std::vector<std::vector<double>> covarianceMatrix(const std::vector<PriceSeries> &);
    static double portfolioVariance(const std::vector<std::vector<double>> &, const std::vector<double> &);
    static double expectedPortfolioReturn(const std::vector<double> &, const std::vector<double> &);
    static double computeSharpeRatio(double, double, double);
    static double computeSortinoRatio(double, double, const std::vector<double> &);
    static double computeMaxDrawdown(const std::vector<double> &);
    static int computeMaxRecoveryTime(const std::vector<double> &);
    static std::vector<double> computeRollingStandardDeviation(const std::vector<double> &, bool sample = false);
    static std::vector<double> computeRollingSharpe(const std::vector<double> &, size_t, double);
    static std::vector<double> computeRollingVolatility(const std::vector<double> &, size_t);
    static std::vector<std::vector<double>> computeCorrelationMatrix(const std::vector<PriceSeries> &);
    static std::vector<double> computeRollingSortino(const std::vector<double> &, size_t, double);
    static std::vector<double> simpleMovingAverage(const std::vector<double> &, size_t);
    static std::vector<double> exponentialMovingAverage(const std::vector<double> &, size_t);
    static double computeBeta(const PriceSeries &asset, const PriceSeries &benchmark);
    static double computeAlpha(const PriceSeries &asset, const PriceSeries &benchmark, double riskFreeRate);
    static double computeAlpha(const PriceSeries &portfolio, const PriceSeries &market, double riskFreeRate, double beta);
    static double computeTreynorRatio(double expectedReturns, double riskFreeRate, double portfolioBeta);
    static double computeInformationRatio(std::vector<double> &portfolioReturns, const std::vector<double> benchmarkReturns);
    static double computeCalmarRatio(double annualReturn, double maxDrawdown);
    static double computeOmegaRatio(const std::vector<double> &returns, double threshold);
    static double computeTotalReturn(const PriceSeries &series);
    static double computeAnnualizedReturn(double totalReturn, int numPeriods, int periodsPerYear);
    static double computeAnnualizedVolatility(const std::vector<double> &returns, int periodsPerYear);
    static double computeAverageDrawdown(const std::vector<double> &cumulativeReturns);
    static double computeSterlingRatio(double averageReturn, double riskFreeRate, double averageDrawdown);
    static double computeVolatilitySkew(const std::vector<double> &returns);
    static double computeDownsideCaptureRatio(const std::vector<double> &portfolioReturns, const std::vector<double> &benchmarkReturns);
    static double computeUpsideCaptureRatio(const std::vector<double> &portfolioReturns, const std::vector<double> &benchmarkReturns);
    static double computeSkewness(const std::vector<double> &returns);
    static double computeKurtosis(const std::vector<double> &returns);
    static double computeGainLossRatio(const std::vector<double> &returns);
    static double computeHitRatio(const std::vector<double> &returns);

    static void exportToCSV(const std::string &, const std::vector<std::string> &, const std::vector<std::vector<double>> &);
};
