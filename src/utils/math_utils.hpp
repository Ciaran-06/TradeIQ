#pragma once

#include <vector>
#include <stdexcept>

namespace MathUtils {
    double mean(const std::vector<double>& data);
    double median(std::vector<double> data);
    double variance(const std::vector<double>& data, bool sample = false);
    double standardDeviation(const std::vector<double>& data, bool sample = false);
    double min(const std::vector<double>& data);
    double max(const std::vector<double>& data);
    double skewness(const std::vector<double>& data);
    double kurtosis(const std::vector<double>& data);
    double percentile(std::vector<double> data, double percentile); // percentile in [0, 100]
    std::vector<double> zScoreNormalize(const std::vector<double>& data);
}