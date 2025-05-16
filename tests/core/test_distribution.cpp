/*
computeSkewness
computeKurtosis
*/

#include <gtest/gtest.h>
#include "stats/distribution.hpp"
#include <vector>
#include <limits>
#include <cmath>

using namespace Stats::Distribution;

TEST(DistributionTest, SkewnessNormalDistribution) {
    std::vector<double> data = {-0.01, 0.0, 0.01};
    double skew = computeSkewness(data);
    EXPECT_NEAR(skew, 0.0, 0.01);
}

TEST(DistributionTest, SkewnessThrowsZeroVariance) {
    std::vector<double> same = {0.05, 0.05, 0.05};
    EXPECT_THROW(computeSkewness(same), std::invalid_argument);
}

TEST(DistributionTest, KurtosisTypical) {
    std::vector<double> data = {-0.02, 0.01, 0.03, -0.01, 0.02};
    double k = computeKurtosis(data);
    EXPECT_GE(k, -3.0);
}

TEST(DistributionTest, KurtosisAllSame) {
    std::vector<double> data = {0.03, 0.03, 0.03};
    double k = computeKurtosis(data);
    EXPECT_TRUE(std::isnan(k));
}

TEST(DistributionTest, GainLossRatioStandard) {
    std::vector<double> r = {0.04, -0.02, 0.03, -0.01, 0.05};
    double result = computeGainLossRatio(r);
    EXPECT_NEAR(result, (0.04 + 0.03 + 0.05) / (0.02 + 0.01), 1e-6);
}

TEST(DistributionTest, GainLossRatioAllPositive) {
    std::vector<double> r = {0.01, 0.02, 0.03};
    double result = computeGainLossRatio(r);
    EXPECT_TRUE(std::isinf(result));
}

TEST(DistributionTest, GainLossRatioAllNegative) {
    std::vector<double> r = {-0.01, -0.02};
    double result = computeGainLossRatio(r);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(DistributionTest, GainLossRatioEmpty) {
    std::vector<double> r = {};
    EXPECT_THROW(computeGainLossRatio(r), std::invalid_argument);
}

TEST(DistributionTest, HitRatioTypical) {
    std::vector<double> r = {0.01, -0.02, 0.03, -0.01};
    double result = computeHitRatio(r);
    EXPECT_NEAR(result, 0.5, 0.01);
}

TEST(DistributionTest, HitRatioAllPositive) {
    std::vector<double> r = {0.01, 0.02, 0.03};
    double result = computeHitRatio(r);
    EXPECT_EQ(result, 1.0);
}

TEST(DistributionTest, HitRatioAllNegative) {
    std::vector<double> r = {-0.01, -0.02, -0.03};
    double result = computeHitRatio(r);
    EXPECT_EQ(result, 0.0);
}

TEST(DistributionTest, HitRatioEmpty) {
    std::vector<double> r = {};
    EXPECT_THROW(computeHitRatio(r), std::invalid_argument);
}
