#pragma once

#include <vector>

namespace Stats::Drawdowns {

    double computeMaxDrawdown(const std::vector<double>& cumulativeReturns);

    int computeMaxRecoveryTime(const std::vector<double>& cumulativeReturns);

    double computeAverageDrawdown(const std::vector<double>& cumulativeReturns);

}
