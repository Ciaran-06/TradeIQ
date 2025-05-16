/*
computeUpsideCaptureRatio
computeDownsideCaptureRatio
*/
#include <gtest/gtest.h>
#include "stats/capture.hpp"
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace Stats::Capture;

TEST(CaptureTest, UpsideCapture_NormalCase) {
    std::vector<double> p = {0.1, 0.2, 0.15};
    std::vector<double> b = {0.1, 0.2, 0.1};
    double result = computeUpsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 1.125, 1e-3);
}

TEST(CaptureTest, DownsideCapture_NormalCase) {
    std::vector<double> p = {-0.1, -0.2, -0.15};
    std::vector<double> b = {-0.1, -0.2, -0.1};
    double result = computeDownsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 1.125, 1e-3);
}

TEST(CaptureTest, UpsideCapture_MixedSigns) {
    std::vector<double> p = {0.1, -0.1, 0.15};
    std::vector<double> b = {0.1, -0.2, 0.1};
    double result = computeUpsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 1.25, 1e-3);
}

TEST(CaptureTest, DownsideCapture_MixedSigns) {
    std::vector<double> p = {0.1, -0.1, 0.15};
    std::vector<double> b = {0.1, -0.2, 0.1};
    double result = computeDownsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 0.5, 1e-3);
}

TEST(CaptureTest, UpsideCapture_NoPositiveBenchmark) {
    std::vector<double> p = {0.1, 0.2};
    std::vector<double> b = {-0.1, -0.2};
    double result = computeUpsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 0.0, 1e-6);
}

TEST(CaptureTest, DownsideCapture_NoNegativeBenchmark) {
    std::vector<double> p = {0.1, 0.2};
    std::vector<double> b = {0.1, 0.2};
    double result = computeDownsideCaptureRatio(p, b);
    EXPECT_NEAR(result, 0.0, 1e-6);
}

TEST(CaptureTest, Capture_EmptyInputs) {
    std::vector<double> p, b;
    EXPECT_NEAR(computeUpsideCaptureRatio(p, b), 0.0, 1e-6);
    EXPECT_NEAR(computeDownsideCaptureRatio(p, b), 0.0, 1e-6);
}

TEST(CaptureTest, MismatchedLengthThrows) {
    std::vector<double> p = {0.1, 0.2};
    std::vector<double> b = {0.1};
    EXPECT_THROW(computeUpsideCaptureRatio(p, b), std::invalid_argument);
    EXPECT_THROW(computeDownsideCaptureRatio(p, b), std::invalid_argument);
}
