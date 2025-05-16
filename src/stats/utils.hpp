#pragma once

#include <vector>

namespace Stats::Utils {

    double computeSkewness(const std::vector<double> &returns);

    double computeKurtosis(const std::vector<double> &returns);

    double computeGainLossRatio(const std::vector<double> &returns);

    double computeHitRatio(const std::vector<double> &returns);
}