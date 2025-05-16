#pragma once

#include "core/price_series.hpp"
#include <vector>

namespace Stats::Correlation
{

    std::vector<std::vector<double>> computeCorrelationMatrix(const std::vector<PriceSeries> &assets);

}
