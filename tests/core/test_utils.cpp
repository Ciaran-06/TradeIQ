/*
simpleMovingAverage
exponentialMovingAverage
exportToCSV
*/

#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <limits>
#include <stdexcept>
#include "stats/utils.hpp"

using namespace Stats::Utils;

TEST(UtilsTest, ComputeSkewness_Normal) {
    std::vector<double> returns = {-0.01, 0.0, 0.01};
    double skew = computeSkewness(returns);
    EXPECT_NEAR(skew, 0.0, 0.1);
}

TEST(UtilsTest, ComputeSkewness_ThrowsOnZeroVariance) {
    std::vector<double> returns = {0.05, 0.05, 0.05};
    EXPECT_THROW(computeSkewness(returns), std::invalid_argument);
}

TEST(UtilsTest, ComputeSkewness_ThrowsOnSmallSample) {
    std::vector<double> returns = {0.01};
    EXPECT_THROW(computeSkewness(returns), std::invalid_argument);
}

TEST(UtilsTest, ComputeKurtosis_NormalData) {
    std::vector<double> returns = {-0.02, 0.01, 0.03, -0.01, 0.02};
    double k = computeKurtosis(returns);
    EXPECT_GE(k, -3.0);  // Excess kurtosis
}

TEST(UtilsTest, ComputeKurtosis_SameValuesReturnsNaN) {
    std::vector<double> returns = {0.03, 0.03, 0.03, 0.03};
    double k = computeKurtosis(returns);
    EXPECT_TRUE(std::isnan(k));
}

TEST(UtilsTest, ComputeKurtosis_ThrowsOnSmallSample) {
    std::vector<double> returns = {0.01, 0.02, 0.03};
    EXPECT_THROW(computeKurtosis(returns), std::invalid_argument);
}

TEST(UtilsTest, ComputeGainLossRatio_Normal) {
    std::vector<double> returns = {0.04, -0.02, 0.03, -0.01, 0.05};
    double result = computeGainLossRatio(returns);
    EXPECT_NEAR(result, (0.04 + 0.03 + 0.05) / (0.02 + 0.01), 1e-6);
}

TEST(UtilsTest, ComputeGainLossRatio_AllPositiveReturnsInfinity) {
    std::vector<double> returns = {0.01, 0.02, 0.03};
    double result = computeGainLossRatio(returns);
    EXPECT_TRUE(std::isinf(result));
}

TEST(UtilsTest, ComputeGainLossRatio_AllNegativeReturnsZero) {
    std::vector<double> returns = {-0.01, -0.02};
    double result = computeGainLossRatio(returns);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(UtilsTest, ComputeGainLossRatio_ThrowsOnEmpty) {
    std::vector<double> returns = {};
    EXPECT_THROW(computeGainLossRatio(returns), std::invalid_argument);
}

TEST(UtilsTest, ComputeHitRatio_Typical) {
    std::vector<double> returns = {0.01, -0.02, 0.03, -0.01};
    double result = computeHitRatio(returns);
    EXPECT_NEAR(result, 0.5, 0.01);
}

TEST(UtilsTest, ComputeHitRatio_AllPositive) {
    std::vector<double> returns = {0.01, 0.02, 0.03};
    double result = computeHitRatio(returns);
    EXPECT_DOUBLE_EQ(result, 1.0);
}

TEST(UtilsTest, ComputeHitRatio_AllNegative) {
    std::vector<double> returns = {-0.01, -0.02, -0.03};
    double result = computeHitRatio(returns);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST(UtilsTest, ComputeHitRatio_ThrowsOnEmpty) {
    std::vector<double> returns = {};
    EXPECT_THROW(computeHitRatio(returns), std::invalid_argument);
}
