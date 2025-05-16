#include "stats/distribution.hpp"
#include <stdexcept>
#include <cmath>
#include <numeric>
#include <unordered_set>

namespace Stats::Distribution {

double computeSkewness(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Empty return series");

    std::unordered_set<double> s(returns.begin(), returns.end());
    if (s.size() == 1) throw std::invalid_argument("All Elements are identical"); 


    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double sum_sq = 0.0, sum_cube = 0.0;
    for (double r : returns) {
        double dev = r - mean;
        sum_sq += dev * dev;
        sum_cube += dev * dev * dev;
    }
    double n = static_cast<double>(returns.size());
    if (sum_sq == 0.0) throw std::invalid_argument("Zero variance");
    return (n / ((n - 1) * (n - 2))) * (sum_cube / std::pow(sum_sq / (n - 1), 1.5));
}

double computeKurtosis(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Empty return series");
    double mean = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double sum_sq = 0.0, sum_quad = 0.0;
    for (double r : returns) {
        double dev = r - mean;
        sum_sq += dev * dev;
        sum_quad += dev * dev * dev * dev;
    }
    double n = static_cast<double>(returns.size());
    if (sum_sq == 0.0) return std::numeric_limits<double>::quiet_NaN();  // Undefined
    double variance = sum_sq / n;
    double kurtosis = (sum_quad / n) / (variance * variance);
    return kurtosis - 3.0;  // Excess kurtosis
}

double computeGainLossRatio(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Empty return series");
    double gains = 0.0, losses = 0.0;
    for (double r : returns) {
        if (r > 0) gains += r;
        else if (r < 0) losses -= r;
    }
    if (losses == 0.0) return std::numeric_limits<double>::infinity();
    return gains / losses;
}

double computeHitRatio(const std::vector<double>& returns) {
    if (returns.empty()) throw std::invalid_argument("Empty return series");
    int hits = 0;
    for (double r : returns)
        if (r > 0) ++hits;
    return static_cast<double>(hits) / returns.size();
}

}
