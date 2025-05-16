/*
computeMaxDrawdown
computeAverageDrawdown
computeMaxRecoveryTime
*/

#include <gtest/gtest.h>
#include "stats/drawdowns.hpp"
#include <vector>
#include <cmath>

using namespace Stats::Drawdowns;

TEST(DrawdownsTest, MaxDrawdown_Basic) {
    std::vector<double> cumReturns = {1.0, 1.1, 0.95, 0.92, 1.05, 1.2, 1.0};
    double result = computeMaxDrawdown(cumReturns);
    EXPECT_NEAR(result, (1.2 - 1.0) / 1.2, 1e-6);  // 0.166666...
}

TEST(DrawdownsTest, MaxDrawdown_NoDrawdown) {
    std::vector<double> cumReturns = {1.0, 1.1, 1.2, 1.3};
    EXPECT_NEAR(computeMaxDrawdown(cumReturns), 0.0, 1e-8);
}

TEST(DrawdownsTest, MaxDrawdown_Empty) {
    std::vector<double> empty;
    EXPECT_NEAR(computeMaxDrawdown(empty), 0.0, 1e-8);
}

TEST(DrawdownsTest, MaxRecoveryTime_Basic) {
    std::vector<double> cumReturns = {100, 95, 90, 92, 97, 100};
    int result = computeMaxRecoveryTime(cumReturns);
    EXPECT_EQ(result, 4);  // From 90 to 100
}

TEST(DrawdownsTest, MaxRecoveryTime_MultipleDrawdowns) {
    std::vector<double> cumReturns = {100, 98, 100, 95, 97, 100, 90, 95, 100};
    int result = computeMaxRecoveryTime(cumReturns);
    EXPECT_EQ(result, 2);  // 90 (idx 6) → 100 (idx 8): 2 steps
}

TEST(DrawdownsTest, MaxRecoveryTime_NoDrawdown) {
    std::vector<double> cumReturns = {100, 105, 110};
    EXPECT_EQ(computeMaxRecoveryTime(cumReturns), 0);
}

TEST(DrawdownsTest, MaxRecoveryTime_EndsInDrawdown) {
    std::vector<double> cumReturns = {100, 90, 95, 97};  // never gets back to 100
    int result = computeMaxRecoveryTime(cumReturns);
    EXPECT_EQ(result, 3);
}

TEST(DrawdownsTest, MaxRecoveryTime_EmptyInput) {
    std::vector<double> empty;
    EXPECT_EQ(computeMaxRecoveryTime(empty), 0);
}

TEST(DrawdownsTest, AverageDrawdown_SingleDip) {
    std::vector<double> returns = {1.0, 1.2, 1.1, 1.3, 1.4};
    double result = computeAverageDrawdown(returns);
    EXPECT_NEAR(result, (1.2 - 1.1) / 1.2, 1e-4);
}

TEST(DrawdownsTest, AverageDrawdown_MultipleDips) {
    std::vector<double> returns = {1.0, 1.2, 1.0, 1.3, 1.1, 1.4};
    double d1 = (1.2 - 1.0) / 1.2;
    double d2 = (1.3 - 1.1) / 1.3;
    double expected = (d1 + d2) / 2.0;
    double result = computeAverageDrawdown(returns);
    EXPECT_NEAR(result, expected, 1e-4);
}

TEST(DrawdownsTest, AverageDrawdown_NoDrawdown) {
    std::vector<double> returns = {1.0, 1.1, 1.2};
    EXPECT_NEAR(computeAverageDrawdown(returns), 0.0, 1e-8);
}

TEST(DrawdownsTest, AverageDrawdown_EndsInDrawdown) {
    std::vector<double> returns = {1.0, 1.2, 1.1};
    double result = computeAverageDrawdown(returns);
    EXPECT_NEAR(result, (1.2 - 1.1) / 1.2, 1e-6);
}

TEST(DrawdownsTest, AverageDrawdown_Empty) {
    std::vector<double> returns;
    EXPECT_NEAR(computeAverageDrawdown(returns), 0.0, 1e-6);
}
