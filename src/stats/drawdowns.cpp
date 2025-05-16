#include "stats/drawdowns.hpp"
#include <limits>
#include <stdexcept>
#include <cmath>

namespace Stats::Drawdowns {

double computeMaxDrawdown(const std::vector<double>& cumulativeReturns) {
    if (cumulativeReturns.empty()) return 0.0;

    double peak = cumulativeReturns[0];
    double maxDrawdown = 0.0;

    for (const auto& val : cumulativeReturns) {
        if (val > peak)
            peak = val;
        double drawdown = (peak - val) / peak;
        if (drawdown > maxDrawdown)
            maxDrawdown = drawdown;
    }

    return maxDrawdown;
}

int computeMaxRecoveryTime(const std::vector<double>& cumulativeReturns) {
    if (cumulativeReturns.empty()) return 0;

    double peak = cumulativeReturns[0];
    size_t peakIndex = 0;
    int maxRecovery = 0;

    for (size_t i = 1; i < cumulativeReturns.size(); ++i) {
        if (cumulativeReturns[i] > peak) {
            peak = cumulativeReturns[i];
            peakIndex = i;
        } else {
            size_t recoveryIndex = i;
            while (recoveryIndex < cumulativeReturns.size() &&
                   cumulativeReturns[recoveryIndex] < peak) {
                ++recoveryIndex;
            }

            if (recoveryIndex < cumulativeReturns.size()) {
                int recoveryTime = static_cast<int>(recoveryIndex - i + 1);
                if (recoveryTime > maxRecovery)
                    maxRecovery = recoveryTime;
            } else {
                int remaining = static_cast<int>(cumulativeReturns.size() - i);
                if (remaining > maxRecovery)
                    maxRecovery = remaining;
            }
        }
    }

    return maxRecovery;
}

double computeAverageDrawdown(const std::vector<double>& cumulativeReturns) {
    if (cumulativeReturns.empty()) return 0.0;

    double peak = cumulativeReturns[0];
    double drawdownSum = 0.0;
    int drawdownCount = 0;
    bool inDrawdown = false;
    double currentDrawdown = 0.0;

    for (const auto& val : cumulativeReturns) {
        if (val > peak) {
            if (inDrawdown) {
                drawdownSum += currentDrawdown;
                drawdownCount++;
                inDrawdown = false;
            }
            peak = val;
        } else {
            inDrawdown = true;
            currentDrawdown = (peak - val) / peak;
        }
    }

    if (inDrawdown) {
        drawdownSum += currentDrawdown;
        drawdownCount++;
    }

    return drawdownCount == 0 ? 0.0 : drawdownSum / drawdownCount;
}

} // namespace Stats::Drawdowns
