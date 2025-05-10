#pragma once

#include <vector>
#include "price_series.hpp"

class StatsEngine
{
public:
    static std::vector<double> computeDailyReturns(const PriceSeries &series);
    static std::vector<double> meanReturns(const std::vector<PriceSeries> &seriesList);
    static std::vector<std::vector<double>> covarianceMatrix(const std::vector<PriceSeries> &seriesList);
    static double portfolioVariance(
    const std::vector<std::vector<double>>& covMatrix,
    const std::vector<double>& weights);

};