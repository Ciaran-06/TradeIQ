#pragma once

#include <vector>

namespace Stats::Ratios
{

    double computeSharpeRatio(double expectedReturn, double variance, double riskFreeRate);

    double computeSortinoRatio(double expectedReturn, double riskFreeRate, const std::vector<double> &returns);

    double computeTreynorRatio(double expectedReturns, double riskFreeRate, double portfolioBeta);

    double computeCalmarRatio(double annualReturn, double maxDrawdown);

    double computeSterlingRatio(double averageReturn, double riskFreeRate, double averageDrawdown);

    double computeInformationRatio(std::vector<double> &portfolioReturns, const std::vector<double> benchmarkReturns);

    double computeOmegaRatio(const std::vector<double> &returns, double threshold);

    double computePortfolioVariance(const std::vector<std::vector<double>> &cov, const std::vector<double> &weights);

}
