/*
covarianceMatrix
computeCorrelationMatrix
computeBeta
computeAlpha (both overloads)
*/

#include <gtest/gtest.h>
#include <TestHelpers.hpp> 
#include "stats/correlation.hpp"

using namespace Stats::Correlation;

TEST(CorrelationTest, IdentityOnSameAssets) {
    auto a = generateSeriesFromReturns("A", {0.1, -0.05, 0.2, -0.1});
    std::vector<PriceSeries> assets = {a, a};

    auto corr = computeCorrelationMatrix(assets);

    ASSERT_EQ(corr.size(), 2);
    EXPECT_NEAR(corr[0][0], 1.0, 1e-6);
    EXPECT_NEAR(corr[1][1], 1.0, 1e-6);
    EXPECT_NEAR(corr[0][1], 1.0, 1e-6);
    EXPECT_NEAR(corr[1][0], 1.0, 1e-6);
}

TEST(CorrelationTest, InverseCorrelation) {
    auto a = generateSeriesFromReturns("A", {0.1, -0.05, 0.2, -0.1});
    auto b = generateInverseSeriesFromReturns("B", {0.1, -0.05, 0.2, -0.1});
    std::vector<PriceSeries> assets = {a, b};

    auto corr = computeCorrelationMatrix(assets);
    EXPECT_NEAR(corr[0][1], -1.0, 1e-6);
    EXPECT_NEAR(corr[1][0], -1.0, 1e-6);
}

TEST(CorrelationTest, MixedCorrelation) {
    auto a = generateSeriesFromReturns("A", {0.01, 0.02, 0.03, 0.04});
    auto b = generateSeriesFromReturns("B", {0.02, 0.04, 0.06, 0.08});    // perfectly correlated with a
    auto c = generateSeriesFromReturns("C", {-0.01, -0.02, -0.03, -0.04}); // perfectly inversely correlated

    auto result = computeCorrelationMatrix({a, b, c});
    EXPECT_NEAR(result[0][1], 1.0, 1e-6);   // a ~ b
    EXPECT_NEAR(result[0][2], -1.0, 1e-6);  // a ~ -c
    EXPECT_NEAR(result[1][2], -1.0, 1e-6);  // b ~ -c
}

TEST(CorrelationTest, MixedCorrelationRnd) {
    auto a = generateSeriesFromReturns("A", {0.01, 0.02, -0.01, 0.03, -0.02});
    auto b = generateSeriesFromReturns("B", {0.02, 0.04, -0.02, 0.06, -0.04});  // Scaled version of `a`
    auto c = generateSeriesFromReturns("C", {-0.01, -0.02, 0.01, -0.03, 0.02}); // Inverse of `a`

    auto result = computeCorrelationMatrix({a, b, c});

    // These won't be exactly ±1 due to floating-point error and realistic variation
    EXPECT_NEAR(result[0][1], 1.0, 1e-8);    // a and b should be strongly correlated
    EXPECT_NEAR(result[0][2], -1.0, 1e-8);   // a and c should be strongly negatively correlated
    EXPECT_NEAR(result[1][2], -1.0, 1e-8);   // b and c too
}


TEST(CorrelationTest, EmptyAssets) {
    std::vector<PriceSeries> empty;
    auto result = computeCorrelationMatrix(empty);
    EXPECT_EQ(result.size(), 0);
}

TEST(CorrelationTest, ZeroVarianceAsset) {
    auto flat = generateFlatSeries("FLAT", 5);
    auto noisy = generateNoisySeries("NOISY", 5);

    auto corr = computeCorrelationMatrix({flat, noisy});
    EXPECT_NEAR(corr[0][1], 0.0, 1e-6);  // Flat asset = zero variance
    EXPECT_NEAR(corr[1][0], 0.0, 1e-6);
    EXPECT_NEAR(corr[0][0], 1.0, 1e-6);
}
