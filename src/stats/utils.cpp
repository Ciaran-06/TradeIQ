#include "stats/utils.hpp"
#include <cmath>
#include <stdexcept>
#include <numeric>
#include <unordered_set>

namespace Stats::Utils {

double computeSkewness(const std::vector<double> &returns) {
    const size_t n = returns.size();
    if (n < 3)
        throw std::invalid_argument("Skewness requires at least 3 data points.");

    std::unordered_set<double> s(returns.begin(), returns.end());
    if(s.size() == 1 ) throw std::invalid_argument("All elements in this vector are identical");
    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / n;

    double m2 = 0.0, m3 = 0.0;
    for (double x : returns) {
        double diff = x - mean;
        m2 += diff * diff;
        m3 += diff * diff * diff;
    }

    m2 /= n;
    m3 /= n;

    if (m2 == 0.0)
        throw std::invalid_argument("Skewness undefined for zero variance.");

    return m3 / std::pow(m2, 1.5);
}

double computeKurtosis(const std::vector<double> &returns) {
    const size_t n = returns.size();
    if (n < 4)
        throw std::invalid_argument("Kurtosis requires at least 4 data points.");

    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / n;

    double m2 = 0.0, m4 = 0.0;
    for (double x : returns) {
        double diff = x - mean;
        m2 += diff * diff;
        m4 += diff * diff * diff * diff;
    }

    m2 /= n;
    m4 /= n;

    if (m2 == 0.0)
        return std::numeric_limits<double>::quiet_NaN();  // undefined

    return m4 / (m2 * m2) - 3.0; // excess kurtosis
}

double computeGainLossRatio(const std::vector<double> &returns) {
    if (returns.empty())
        throw std::invalid_argument("Returns cannot be empty.");

    double gains = 0.0, losses = 0.0;

    for (double r : returns) {
        if (r > 0.0)
            gains += r;
        else if (r < 0.0)
            losses += std::abs(r);
    }

    if (losses == 0.0)
        return std::numeric_limits<double>::infinity();
    return gains / losses;
}

double computeHitRatio(const std::vector<double> &returns) {
    const size_t n = returns.size();
    if (n == 0)
        throw std::invalid_argument("Returns cannot be empty.");

    size_t hits = 0;
    for (double r : returns) {
        if (r > 0.0)
            ++hits;
    }

    return static_cast<double>(hits) / n;
}

} 
