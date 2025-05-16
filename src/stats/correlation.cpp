#include "stats/correlation.hpp"
#include "stats/returns.hpp"
#include "../utils/math_utils.hpp"

// delete for debugging
#include <iostream>

namespace Stats::Correlation
{

    std::vector<std::vector<double>> computeCorrelationMatrix(const std::vector<PriceSeries> &assets)
    {
        size_t n = assets.size();
        std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 1.0));

        if (n == 0)
            return matrix;

        std::vector<std::vector<double>> returns(n);
        for (size_t i = 0; i < n; ++i)
        {
            returns[i] = assets[i].getDailyReturns();
        }

        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = i + 1; j < n; ++j)
            {
                const auto &r1 = returns[i];
                const auto &r2 = returns[j];
                size_t len = std::min(r1.size(), r2.size());

                if (len < 2)
                {
                    matrix[i][j] = matrix[j][i] = 0.0;
                    continue;
                }

                std::vector<double>  sub_r1(r1.begin(), r1.begin() + len);
                std::vector<double>  sub_r2(r2.begin(), r2.begin() + len);

                double mean1 = MathUtils::mean(sub_r1);
                double mean2 = MathUtils::mean(sub_r2);
                
                double numerator = 0.0, denom1 = 0.0, denom2 = 0.0;
                for (size_t k = 0; k < len; ++k)
                {
                    double d1 = r1[k] - mean1;
                    double d2 = r2[k] - mean2;
                    numerator += d1 * d2;
                    denom1 += d1 * d1;
                    denom2 += d2 * d2;
                }

                double corr = (denom1 > 0 && denom2 > 0)
                                  ? numerator / std::sqrt(denom1 * denom2)
                                  : 0.0;

                matrix[i][j] = matrix[j][i] = corr;
            }
        }

        return matrix;
    }
}