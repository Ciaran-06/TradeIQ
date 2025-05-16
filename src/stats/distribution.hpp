#pragma once

#include <vector>

namespace Stats::Distribution {

    double computeSkewness(const std::vector<double>& returns);
    double computeKurtosis(const std::vector<double>& returns);
    double computeGainLossRatio(const std::vector<double>& returns);
    double computeHitRatio(const std::vector<double>& returns);

}
