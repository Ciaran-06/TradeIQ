#include "stats/ratios.hpp"
#include <numeric>
#include <cmath>
#include <stdexcept>

namespace Stats::Ratios {

double computeSharpeRatio(double expectedReturn, double variance, double riskFreeRate) {
    double excessReturn = expectedReturn - riskFreeRate;
    if (variance <= 0.0) return 0.0;
    return excessReturn / std::sqrt(variance);
}

double computeSortinoRatio(double expectedReturn, double riskFreeRate, const std::vector<double> &returns) {
    double downsideSum = 0.0;
    int count = 0;

    for (double r : returns) {
        if (r < riskFreeRate) {
            double diff = r - riskFreeRate;
            downsideSum += diff * diff;
            ++count;
        }
    }

    if (count == 0) return std::numeric_limits<double>::infinity();
    double downsideDev = std::sqrt(downsideSum / count);
    if (downsideDev == 0.0) return std::numeric_limits<double>::infinity();

    return (expectedReturn - riskFreeRate) / downsideDev;
}

double computeTreynorRatio(double expectedReturns, double riskFreeRate, double portfolioBeta) {
    if (portfolioBeta == 0.0) return 0.0;
    return (expectedReturns - riskFreeRate) / portfolioBeta;
}

double computeCalmarRatio(double annualReturn, double maxDrawdown) {
    if (maxDrawdown == 0.0) return std::numeric_limits<double>::infinity();
    return annualReturn / maxDrawdown;
}

double computeSterlingRatio(double averageReturn, double riskFreeRate, double averageDrawdown) {
    if (averageDrawdown == 0.0) return std::numeric_limits<double>::infinity();
    return (averageReturn - riskFreeRate) / averageDrawdown;
}

double computeInformationRatio(std::vector<double> &portfolioReturns, const std::vector<double> benchmarkReturns) {
    size_t n = portfolioReturns.size();
    if (n == 0 || benchmarkReturns.size() != n)
        throw std::invalid_argument("Returns must be non-empty and of equal length.");

    std::vector<double> activeReturns;
    activeReturns.reserve(n);

    for (size_t i = 0; i < n; ++i)
        activeReturns.push_back(portfolioReturns[i] - benchmarkReturns[i]);

    double meanActive = std::accumulate(activeReturns.begin(), activeReturns.end(), 0.0) / n;

    double sqSum = 0.0;
    for (double r : activeReturns)
        sqSum += (r - meanActive) * (r - meanActive);

    double stddev = std::sqrt(sqSum / n);
    if (stddev == 0.0) return 0.0;

    return meanActive / stddev;
}

double computeOmegaRatio(const std::vector<double> &returns, double threshold) {
    double num = 0.0, denom = 0.0;

    for (double r : returns) {
        if (r >= threshold)
            num += (r - threshold);
        else
            denom += (threshold - r);
    }

    if (denom == 0.0) return std::numeric_limits<double>::infinity();
    return num / denom;
}

double computePortfolioVariance(const std::vector<std::vector<double>>& covMatrix,
                                const std::vector<double>& weights) {
    double variance = 0.0;
    size_t n = weights.size();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            variance += weights[i] * covMatrix[i][j] * weights[j];
        }
    }

    return variance;
}

}
