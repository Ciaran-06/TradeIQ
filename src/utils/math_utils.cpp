#include "math_utils.hpp"

double MathUtils::mean(const std::vector<double> &data)
{
    if (data.size() == 0)
        throw std::invalid_argument("data is empty");

    double sum = 0.0;
    for (const auto &r : data)
    {
        sum += r;
    }

    return sum / data.size();
}

double MathUtils::standardDeviation(const std::vector<double> &data, bool sample)
{
    if (data.size() == 0)
        throw std::invalid_argument("Sample can't be empty");

    if (sample && data.size() < 2)
        throw std::invalid_argument("Sample size must be > 1 for sample std::dev");
    double mean = MathUtils::mean(data);

   double variation = 0.0;

   for (const auto& r : data) 
   {
    variation += (r - mean) * (r - mean);
   }

   if (sample) {
    return variation / (data.size() -1);
   } else {
    return variation / data.size();
   }
}
