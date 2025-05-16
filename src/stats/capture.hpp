#pragma once

#include <vector>

namespace Stats::Capture {

    double computeUpsideCaptureRatio(const std::vector<double>& portfolioReturns,
                                     const std::vector<double>& benchmarkReturns);

    double computeDownsideCaptureRatio(const std::vector<double>& portfolioReturns,
                                       const std::vector<double>& benchmarkReturns);
}
