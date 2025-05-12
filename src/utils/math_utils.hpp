#pragma once

#include <vector>

namespace MathUtils {
        double mean(const std::vector<double>& data);
        double standardDeviation(const std::vector<double>& data, bool sample = false);
}