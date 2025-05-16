/*
portfolioVariance
computeAnnualizedVolatility
computeRollingStandardDeviation
computeRollingVolatility
computeVolatilitySkew
*/
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>
#include "stats/volatility.hpp"

using namespace Stats::Volatility;

TEST(VolatilityTest, ComputeAnnualizedVolatility_BasicCase) {
    std::vector<double> returns = {0.01, -0.02, 0.015, -0.005, 0.01};
    int periodsPerYear = 252;

    double mean = 0.002;
    double var = 0.000194;
    double expected = std::sqrt(var) * std::sqrt(periodsPerYear);

    double result = computeAnnualizedVolatility(returns, periodsPerYear);
    EXPECT_NEAR(result, expected, 1e-4);
}

TEST(VolatilityTest, ComputeAnnualizedVolatility_ThrowsOnEmpty) {
    std::vector<double> empty;
    EXPECT_THROW(computeAnnualizedVolatility(empty, 252), std::invalid_argument);
}

TEST(VolatilityTest, ComputeAnnualizedVolatility_ThrowsOnZeroPeriods) {
    std::vector<double> data = {0.01, -0.01};
    EXPECT_THROW(computeAnnualizedVolatility(data, 0), std::invalid_argument);
}

TEST(VolatilityTest, RollingStandardDeviation_CorrectSizeAndValues) {
    std::vector<double> returns = {1.0, 2.0, 3.0, 2.0, 1.0};
    std::vector<double> result = computeRollingStandardDeviation(returns, true);

    ASSERT_EQ(result.size(), 3);
    EXPECT_NEAR(result[0], 1.0, 0.01);
    EXPECT_NEAR(result[1], 0.577, 0.01);
    EXPECT_NEAR(result[2], 1.0, 0.01);
}

TEST(VolatilityTest, RollingStandardDeviation_SampleFalse) {
    std::vector<double> returns = {1.0, 2.0, 3.0};
    std::vector<double> result = computeRollingStandardDeviation(returns, false);
    ASSERT_EQ(result.size(), 1);

    double mean = 2.0;
    double variance = ((1.0 - mean)*(1.0 - mean) + (2.0 - mean)*(2.0 - mean) + (3.0 - mean)*(3.0 - mean)) / 3;
    double expected = std::sqrt(variance);

    EXPECT_NEAR(result[0], expected, 1e-6);
}

TEST(VolatilityTest, RollingStandardDeviation_TooShortInputReturnsEmpty) {
    std::vector<double> returns = {1.0, 2.0};
    std::vector<double> result = computeRollingStandardDeviation(returns);
    EXPECT_TRUE(result.empty());
}

TEST(VolatilityTest, RollingVolatility_BasicCase) {
    std::vector<double> returns = {1.0, 2.0, 3.0, 2.0, 1.0};
    std::vector<double> result = computeRollingVolatility(returns, 3);

    ASSERT_EQ(result.size(), 3);
    EXPECT_NEAR(result[0], 1.0, 0.01);
    EXPECT_NEAR(result[1], 0.577, 0.01);
    EXPECT_NEAR(result[2], 1.0, 0.01);
}

TEST(VolatilityTest, RollingVolatility_WindowTooLarge) {
    std::vector<double> returns = {1.0, 2.0};
    std::vector<double> result = computeRollingVolatility(returns, 5);
    EXPECT_TRUE(result.empty());
}

TEST(VolatilityTest, RollingVolatility_WindowZero) {
    std::vector<double> returns = {1.0, 2.0};
    std::vector<double> result = computeRollingVolatility(returns, 0);
    EXPECT_TRUE(result.empty());
}
