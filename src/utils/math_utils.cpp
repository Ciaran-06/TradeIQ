
#include "math_utils.hpp"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <unordered_set>

namespace MathUtils
{

    double mean(const std::vector<double> &data)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    double median(std::vector<double> data)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        std::sort(data.begin(), data.end());
        size_t n = data.size();
        return (n % 2 == 0) ? (data[n / 2 - 1] + data[n / 2]) / 2.0 : data[n / 2];
    }

    double variance(const std::vector<double> &data, bool sample)
    {
        if (data.size() < (sample ? 2 : 1))
            throw std::invalid_argument("Not enough data.");
        double m = mean(data);
        double sum = 0.0;
        for (double d : data)
        {
            double diff = d - m;
            sum += diff * diff;
        }
        return sum / (sample ? data.size() - 1 : data.size());
    }

    double standardDeviation(const std::vector<double> &data, bool sample)
    {
        return std::sqrt(variance(data, sample));
    }

    double min(const std::vector<double> &data)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        return *std::min_element(data.begin(), data.end());
    }

    double max(const std::vector<double> &data)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        return *std::max_element(data.begin(), data.end());
    }

    double skewness(const std::vector<double> &data)
    {
        if (data.size() < 3)
            throw std::invalid_argument("Need at least 3 data points.");

        double m = mean(data);
        double sd = standardDeviation(data, true);
        double skew = 0.0;
        for (double d : data)
        {
            skew += std::pow((d - m) / sd, 3);
        }
        return skew * (static_cast<double>(data.size()) / ((data.size() - 1) * (data.size() - 2)));
    }

    double kurtosis(const std::vector<double> &data)
    {
        if (data.size() < 4)
            throw std::invalid_argument("Need at least 4 data points.");
        double m = mean(data);
        double sd = standardDeviation(data, true);
        double k = 0.0;
        for (double d : data)
        {
            k += std::pow((d - m) / sd, 4);
        }
        double n = static_cast<double>(data.size());
        return (n * (n + 1) * k - 3 * std::pow(n - 1, 2)) / ((n - 1) * (n - 2) * (n - 3));
    }

    double percentile(std::vector<double> data, double p)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        if (p < 0.0 || p > 100.0)
            throw std::invalid_argument("Percentile must be between 0 and 100.");
        std::sort(data.begin(), data.end());
        double rank = (p / 100.0) * (data.size() - 1);
        size_t lower = static_cast<size_t>(rank);
        size_t upper = lower + 1;
        double weight = rank - lower;
        return upper < data.size()
                   ? data[lower] * (1.0 - weight) + data[upper] * weight
                   : data[lower];
    }

    std::vector<double> zScoreNormalize(const std::vector<double> &data)
    {
        if (data.empty())
            throw std::invalid_argument("Data is empty.");
        double m = mean(data);
        double sd = standardDeviation(data);
        if (sd == 0.0)
            throw std::domain_error("Standard deviation is zero.");
        std::vector<double> result;
        result.reserve(data.size());
        for (double d : data)
        {
            result.push_back((d - m) / sd);
        }
        return result;
    }

}