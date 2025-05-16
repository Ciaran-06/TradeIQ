/*
computeDailyReturns
meanReturns
computeTotalReturn
computeAnnualizedReturn
expectedPortfolioReturn
*/
#include <gtest/gtest.h>
#include "core/price_series.hpp"
#include "stats/returns.hpp"
#include "TestHelpers.hpp"
#include <stdexcept>
#include <cmath>

using namespace Stats::Returns;

TEST(ReturnsTest, ComputeDailyReturns_Normal) {
    PriceSeries ps("TEST", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 110.0, 121.0});
    auto returns = computeDailyReturns(ps);

    ASSERT_EQ(returns.size(), 2);
    EXPECT_NEAR(returns[0], 0.10, 1e-6);
    EXPECT_NEAR(returns[1], 0.10, 1e-6);
}

TEST(ReturnsTest, ComputeDailyReturns_Empty) {
    PriceSeries ps("EMPTY", {}, {});
    EXPECT_THROW(computeDailyReturns(ps), std::invalid_argument);
}

TEST(ReturnsTest, ComputeDailyReturns_SinglePrice) {
    PriceSeries ps("ONE", {"2023-01-01"}, {100.0});
    EXPECT_THROW(computeDailyReturns(ps), std::invalid_argument);
}

TEST(ReturnsTest, ComputeDailyReturns_DivideByZero) {
    PriceSeries ps("ZERO", {"2023-01-01", "2023-01-02"}, {0.0, 100.0});
    EXPECT_THROW(computeDailyReturns(ps), std::invalid_argument);
}

TEST(ReturnsTest, MeanReturns_Normal) {
    std::vector<double> returns = {0.01, 0.03, 0.02};
    double mean = meanReturns(returns);
    EXPECT_NEAR(mean, 0.02, 1e-6);
}

TEST(ReturnsTest, MeanReturns_Empty) {
    std::vector<double> returns = {};
    EXPECT_THROW(meanReturns(returns), std::invalid_argument);
}

TEST(ReturnsTest, ComputeTotalReturn_Normal) {
    PriceSeries ps("TEST", {"", "", ""}, {100, 110, 121});
    double total = computeTotalReturn(ps);
    EXPECT_NEAR(total, 0.21, 1e-6);
}

TEST(ReturnsTest, ComputeTotalReturn_SinglePoint) {
    PriceSeries ps("ONE", {""}, {100});  // One price only
    EXPECT_THROW(computeTotalReturn(ps), std::invalid_argument);
}

TEST(ReturnsTest, ComputeTotalReturn_ZeroStart) {
    PriceSeries ps("BAD", {"", "", ""}, {0.0, 10.0, 20.0});
    EXPECT_THROW(computeTotalReturn(ps), std::invalid_argument);
}

TEST(ReturnsTest, ComputeAnnualizedReturn_OneYear) {
    double result = computeAnnualizedReturn(0.10, 12, 12);
    EXPECT_NEAR(result, 0.10, 1e-6);
}

TEST(ReturnsTest, ComputeAnnualizedReturn_MultiYear) {
    double result = computeAnnualizedReturn(0.21, 24, 12); // 2 years, monthly
    EXPECT_NEAR(result, 0.10, 0.01);
}

TEST(ReturnsTest, ComputeAnnualizedReturn_Daily) {
    double result = computeAnnualizedReturn(0.05, 252, 252);
    EXPECT_NEAR(result, 0.05, 1e-6);
}

TEST(ReturnsTest, ComputeAnnualizedReturn_ZeroPeriods) {
    EXPECT_THROW(computeAnnualizedReturn(0.1, 0, 12), std::invalid_argument);
}

TEST(ReturnsTest, ComputeAnnualizedReturn_InvalidPeriodInputs) {
    EXPECT_THROW(computeAnnualizedReturn(0.1, -1, 12), std::invalid_argument);
    EXPECT_THROW(computeAnnualizedReturn(0.1, 12, 0), std::invalid_argument);
}

TEST(ReturnsTest, ExpectedPortfolioReturn_ValidInput) {
    std::vector<double> means = {0.01, 0.03};
    std::vector<double> weights = {0.5, 0.5};
    double result = expectedPortfolioReturn(means, weights);
    EXPECT_NEAR(result, 0.02, 1e-6);
}

TEST(ReturnsTest, ExpectedPortfolioReturn_MismatchedLengths) {
    std::vector<double> means = {0.01, 0.03};
    std::vector<double> weights = {0.5};
    EXPECT_THROW(expectedPortfolioReturn(means, weights), std::invalid_argument);
}

TEST(ReturnsTest, ExpectedPortfolioReturn_EmptyVectors) {
    std::vector<double> means = {};
    std::vector<double> weights = {};
    double result = expectedPortfolioReturn(means, weights);
    EXPECT_NEAR(result, 0.0, 1e-6); // nothing to multiply
}
