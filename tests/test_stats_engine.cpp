#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "price_series.hpp"
#include "stats_engine.hpp"

TEST_CASE("Daily returns are computed correctly", "[stats]") {
    PriceSeries ps("TEST", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 105.0, 110.25});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 2);
    REQUIRE(returns[0] == Catch::Approx(0.05));      // 5%
    REQUIRE(returns[1] == Catch::Approx(0.05));      // 5%
}

TEST_CASE("Mean returns are computed correctly", "[stats]") {
    PriceSeries p1("AAPL", {"d1", "d2", "d3"}, {100, 105, 110.25}); // 5% + 5%
    PriceSeries p2("MSFT", {"d1", "d2", "d3"}, {100, 102, 104.04}); // 2% + 2%

    std::vector<PriceSeries> seriesList = {p1, p2};
    auto means = StatsEngine::meanReturns(seriesList);

    REQUIRE(means.size() == 2);
    REQUIRE(means[0] == Catch::Approx(0.05));
    REQUIRE(means[1] == Catch::Approx(0.02));
}

TEST_CASE("Covariance matrix is computed correctly", "[stats]") {
    // Two perfectly correlated assets
    PriceSeries a("A", {"d1", "d2", "d3"}, {100, 105, 110});
    PriceSeries b("B", {"d1", "d2", "d3"}, {200, 210, 220});

    std::vector<PriceSeries> input = {a, b};
    auto matrix = StatsEngine::covarianceMatrix(input);

    REQUIRE(matrix.size() == 2);
    REQUIRE(matrix[0].size() == 2);
    REQUIRE(matrix[0][0] == Catch::Approx(matrix[1][1])); // same variance
    REQUIRE(matrix[0][1] == Catch::Approx(matrix[1][0])); // symmetry
    REQUIRE(matrix[0][1] > 0); // positively correlated
}

TEST_CASE("Covariance matrix - perfect correlation", "[stats]") {
    // Asset A: 100 → 110 → 121 (10% per day)
    // Asset B: 200 → 220 → 242 (also 10% per day)
    PriceSeries a("A", {"d1", "d2", "d3"}, {100.0, 110.0, 121.0});
    PriceSeries b("B", {"d1", "d2", "d3"}, {200.0, 220.0, 242.0});

    std::vector<PriceSeries> assets = {a, b};

    auto matrix = StatsEngine::covarianceMatrix(assets);

    REQUIRE(matrix.size() == 2);
    REQUIRE(matrix[0].size() == 2);

    // Variance of a 10% return series over two periods
    double expectedVar = (0.1 - 0.1) * (0.1 - 0.1) + (0.1 - 0.1) * (0.1 - 0.1);
    expectedVar = 0.0 / 1;  // all values = mean = 10%, so zero variance

    // But in real terms, tiny floating-point errors may result in ≠ 0, so check against Approx
    REQUIRE(matrix[0][0] == Catch::Approx(matrix[1][1]));
    REQUIRE(matrix[0][1] == Catch::Approx(matrix[1][0]));
    REQUIRE(matrix[0][0] == Catch::Approx(matrix[0][1])); // perfect correlation
}

TEST_CASE("Portfolio variance is computed correctly", "[portfolio][variance]") {
    std::vector<std::vector<double>> covMatrix = {
        {0.01, 0.005},
        {0.005, 0.02}
    };

    std::vector<double> weights = {0.5, 0.5};

    double variance = StatsEngine::portfolioVariance(covMatrix, weights);

    REQUIRE(variance == Catch::Approx(0.01).epsilon(1e-6));
}


TEST_CASE("Expected portfolio return is computed correctly", "[portfolio][return]") {
    std::vector<double> meanReturns = {0.01, 0.03};
    std::vector<double> weights = {0.5, 0.5};

    double result = StatsEngine::expectedPortfolioReturn(meanReturns, weights);

    REQUIRE(result == Catch::Approx(0.02).epsilon(1e-6));
}

TEST_CASE("Sharpe ratio is calculated correctly", "[sharpe]") {
    double expectedReturn = 0.10;
    double variance = 0.04;
    double riskFree = 0.02;

    double result = StatsEngine::computeSharpeRatio(expectedReturn, variance, riskFree);

    REQUIRE(result == Catch::Approx(0.4).epsilon(1e-6));
}

TEST_CASE("Sortino ratio is computed correctly", "[sortino]") {
    std::vector<double> returns = {0.01, 0.03, -0.02, -0.01, 0.04};
    double expectedReturn = 0.01;
    double riskFree = 0.0;

    double result = StatsEngine::computeSortinoRatio(expectedReturn, riskFree, returns);

    REQUIRE(result == Catch::Approx(0.632).epsilon(0.001));
}

TEST_CASE("Max drawdown is computed correctly", "[drawdown]") {
    std::vector<double> cumulativeReturns = {1.0, 1.1, 0.95, 0.92, 1.05, 1.2, 1.0};

    double result = StatsEngine::computeMaxDrawdown(cumulativeReturns);

    REQUIRE(result == Catch::Approx(1.0 / 6.0).epsilon(0.00001));
}