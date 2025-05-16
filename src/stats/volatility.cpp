#include "stats/volatility.hpp"
#include "ratios.hpp"
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace Stats::Volatility
{

    double computeAnnualizedVolatility(const std::vector<double> &returns, int periodsPerYear)
    {
        if (returns.empty())
            throw std::invalid_argument("Returns vector cannot be empty.");
        if (periodsPerYear <= 0)
            throw std::invalid_argument("Periods per year must be positive.");

        double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
        double sumSquares = 0.0;

        for (double r : returns)
        {
            double diff = r - mean;
            sumSquares += diff * diff;
        }

        double variance = sumSquares / (returns.size() - 1);
        return std::sqrt(variance) * std::sqrt(periodsPerYear);
    }

    std::vector<double> computeRollingStandardDeviation(const std::vector<double> &returns, bool sample)
    {
        size_t window = 3;
        if (returns.size() < window)
            return {};

        std::vector<double> result;
        result.reserve(returns.size() - window + 1);

        for (size_t i = 0; i <= returns.size() - window; ++i)
        {
            double sum = 0.0, sumSq = 0.0;

            for (size_t j = 0; j < window; ++j)
            {
                double val = returns[i + j];
                sum += val;
                sumSq += val * val;
            }

            double mean = sum / window;
            double variance = (sumSq - window * mean * mean) / (sample ? (window - 1) : window);
            result.push_back(std::sqrt(variance));
        }

        return result;
    }

    std::vector<double> computeRollingVolatility(const std::vector<double> &returns, size_t window)
    {
        if (returns.size() < window || window == 0)
            return {};

        std::vector<double> result;
        result.reserve(returns.size() - window + 1);

        for (size_t i = 0; i <= returns.size() - window; ++i)
        {
            double sum = 0.0, sumSq = 0.0;

            for (size_t j = 0; j < window; ++j)
            {
                double val = returns[i + j];
                sum += val;
                sumSq += val * val;
            }

            double mean = sum / window;
            double variance = (sumSq - window * mean * mean) / (window - 1);
            result.push_back(std::sqrt(variance));
        }

        return result;
    }

    std::vector<double> computeRollingSharpe(const std::vector<double> &returns, int windowSize, double riskFreeRate)
    {
        std::vector<double> rollingSharpe;

        for (size_t i = 0; i + windowSize <= returns.size(); ++i)
        {
            std::vector<double> window(returns.begin() + i, returns.begin() + i + windowSize);

            double sum = std::accumulate(window.begin(), window.end(), 0.0);
            double mean = sum / windowSize;

            double variance = 0.0;
            for (const auto &val : window)
                variance += (val - mean) * (val - mean);
            variance /= windowSize;

            double sharpe = Stats::Ratios::computeSharpeRatio(mean, variance, riskFreeRate);
            rollingSharpe.push_back(sharpe);
        }

        return rollingSharpe;
    }

} // namespace Stats::Volatility
