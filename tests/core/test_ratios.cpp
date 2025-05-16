/*
computeSharpeRatio
computeSortinoRatio
computeRollingSharpe
computeRollingSortino
computeTreynorRatio
computeCalmarRatio
computeOmegaRatio
computeSterlingRatio
computeGainLossRatio
computeHitRatio
*/

#include <gtest/gtest.h>
#include "stats/ratios.hpp"
#include <limits>
#include <vector>
#include <cmath>
#include <stdexcept>

using namespace Stats::Ratios;

TEST(RatiosTest, SharpeRatio_NormalCase) {
    double result = computeSharpeRatio(0.10, 0.04, 0.02);
    EXPECT_NEAR(result, 0.4, 1e-6);
}

TEST(RatiosTest, SharpeRatio_ZeroVarianceReturnsZero) {
    double result = computeSharpeRatio(0.10, 0.0, 0.02);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(RatiosTest, SortinoRatio_TypicalCase) {
    std::vector<double> returns = {0.01, -0.02, 0.03, -0.01, 0.04};
    double result = computeSortinoRatio(0.01, 0.0, returns);
    EXPECT_NEAR(result, 0.632, 0.01);
}

TEST(RatiosTest, SortinoRatio_NoDownsideReturnsInfinity) {
    std::vector<double> returns = {0.02, 0.03, 0.01};
    double result = computeSortinoRatio(0.03, 0.0, returns);
    EXPECT_TRUE(std::isinf(result));
}

TEST(RatiosTest, TreynorRatio_Normal) {
    double result = computeTreynorRatio(0.12, 0.02, 1.25);
    EXPECT_NEAR(result, 0.08, 1e-6);
}

TEST(RatiosTest, TreynorRatio_ZeroBetaReturnsZero) {
    double result = computeTreynorRatio(0.12, 0.02, 0.0);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(RatiosTest, CalmarRatio_Normal) {
    double result = computeCalmarRatio(0.30, 0.10);
    EXPECT_NEAR(result, 3.0, 1e-6);
}

TEST(RatiosTest, CalmarRatio_ZeroDrawdownInfinity) {
    double result = computeCalmarRatio(0.10, 0.0);
    EXPECT_TRUE(std::isinf(result));
}

TEST(RatiosTest, SterlingRatio_Normal) {
    double result = computeSterlingRatio(0.10, 0.02, 0.05); // (0.08 / 0.05)
    EXPECT_NEAR(result, 1.6, 1e-6);
}

TEST(RatiosTest, SterlingRatio_ZeroDrawdownInfinity) {
    double result = computeSterlingRatio(0.10, 0.02, 0.0);
    EXPECT_TRUE(std::isinf(result));
}

TEST(RatiosTest, InformationRatio_StandardCase) {
    std::vector<double> portfolio = {0.08, 0.10, 0.12, 0.09, 0.11};
    std::vector<double> benchmark = {0.05, 0.05, 0.05, 0.05, 0.05};
    double result = computeInformationRatio(portfolio, benchmark);
    EXPECT_GT(result, 0.0);
}

TEST(RatiosTest, InformationRatio_PerfectMatchReturnsZero) {
    std::vector<double> portfolio = {0.05, 0.05, 0.05};
    std::vector<double> benchmark = {0.05, 0.05, 0.05};
    double result = computeInformationRatio(portfolio, benchmark);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(RatiosTest, InformationRatio_MismatchedLengthsThrows) {
    std::vector<double> a = {0.05, 0.06};
    std::vector<double> b = {0.05};
    EXPECT_THROW(computeInformationRatio(a, b), std::invalid_argument);
}

TEST(RatiosTest, InformationRatio_EmptyThrows) {
    std::vector<double> a = {}, b = {};
    EXPECT_THROW(computeInformationRatio(a, b), std::invalid_argument);
}

TEST(RatiosTest, InformationRatio_ZeroStddevReturnsZero) {
    std::vector<double> portfolio = {0.06, 0.06, 0.06};
    std::vector<double> benchmark = {0.05, 0.05, 0.05};
    double result = computeInformationRatio(portfolio, benchmark);
    EXPECT_NEAR(result, 0.0, 1e-8);
}

TEST(RatiosTest, OmegaRatio_Balanced) {
    std::vector<double> returns = {0.02, 0.04, -0.01, -0.02, 0.03};
    double result = computeOmegaRatio(returns, 0.0);
    EXPECT_NEAR(result, 3.0, 0.1);
}

TEST(RatiosTest, OmegaRatio_AllAboveThresholdReturnsInfinity) {
    std::vector<double> returns = {0.03, 0.04, 0.05};
    double result = computeOmegaRatio(returns, 0.01);
    EXPECT_TRUE(std::isinf(result));
}

TEST(RatiosTest, OmegaRatio_AllBelowThresholdReturnsZero) {
    std::vector<double> returns = {-0.02, -0.01, -0.03};
    double result = computeOmegaRatio(returns, 0.0);
    EXPECT_NEAR(result, 0.0, 1e-6);
}

TEST(RatiosTest, OmegaRatio_MixedWithThreshold) {
    std::vector<double> returns = {0.015, 0.005, -0.005, 0.025};
    double result = computeOmegaRatio(returns, 0.01);
    EXPECT_NEAR(result, 1.0, 0.1);
}
