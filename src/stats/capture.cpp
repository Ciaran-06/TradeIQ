#include "stats/capture.hpp"
#include <stdexcept>
#include <numeric>

namespace Stats::Capture {

double computeUpsideCaptureRatio(const std::vector<double>& portfolioReturns,
                                 const std::vector<double>& benchmarkReturns) {
    if (portfolioReturns.size() != benchmarkReturns.size())
        throw std::invalid_argument("Return vectors must be the same length.");

    double portfolioSum = 0.0;
    double benchmarkSum = 0.0;

    for (size_t i = 0; i < benchmarkReturns.size(); ++i) {
        if (benchmarkReturns[i] > 0.0) {
            portfolioSum += portfolioReturns[i];
            benchmarkSum += benchmarkReturns[i];
        }
    }

    if (benchmarkSum == 0.0)
        return 0.0;

    return portfolioSum / benchmarkSum;
}

double computeDownsideCaptureRatio(const std::vector<double>& portfolioReturns,
                                   const std::vector<double>& benchmarkReturns) {
    if (portfolioReturns.size() != benchmarkReturns.size())
        throw std::invalid_argument("Return vectors must be the same length.");

    double portfolioSum = 0.0;
    double benchmarkSum = 0.0;

    for (size_t i = 0; i < benchmarkReturns.size(); ++i) {
        if (benchmarkReturns[i] < 0.0) {
            portfolioSum += portfolioReturns[i];
            benchmarkSum += benchmarkReturns[i];
        }
    }

    if (benchmarkSum == 0.0)
        return 0.0;

    return portfolioSum / benchmarkSum;
}

}
