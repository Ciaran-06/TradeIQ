#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "core/price_series.hpp"
#include "core/stats_engine.hpp"
#include "TestHelpers.hpp"

TEST_CASE("Daily returns are computed correctly", "[stats]")
{
    PriceSeries ps("TEST", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 105.0, 110.25});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 2);
    REQUIRE(returns[0] == Catch::Approx(0.05));
    REQUIRE(returns[1] == Catch::Approx(0.05));
}

TEST_CASE("Mean returns are computed correctly", "[stats]")
{
    PriceSeries p1 = generateSeriesFromReturns("AAPL", {0.05, 0.05});
    PriceSeries p2 = generateSeriesFromReturns("MSFT", {0.02, 0.02});

    std::vector<double> means;
    for (const auto &ps : {p1, p2})
    {
        auto rets = StatsEngine::computeDailyReturns(ps);
        means.push_back(StatsEngine::meanReturns(rets));
    }

    REQUIRE(means.size() == 2);
    REQUIRE(means[0] == Catch::Approx(0.05));
    REQUIRE(means[1] == Catch::Approx(0.02));
}

TEST_CASE("Covariance matrix is computed correctly", "[stats]")
{
    PriceSeries a = generateSeriesFromReturns("A", {0.05, 0.05});
    PriceSeries b = generateSeriesFromReturns("B", {0.05, 0.05});

    std::vector<PriceSeries> input = {a, b};
    auto matrix = StatsEngine::covarianceMatrix(input);

    REQUIRE(matrix.size() == 2);
    REQUIRE(matrix[0].size() == 2);
    REQUIRE(matrix[0][0] == Catch::Approx(matrix[1][1]));
    REQUIRE(matrix[0][1] == Catch::Approx(matrix[1][0]));
    REQUIRE(matrix[0][1] > 0);
}

TEST_CASE("Covariance matrix - perfect correlation", "[stats]")
{
    PriceSeries a = generateSeriesFromReturns("A", {0.1, 0.1});
    PriceSeries b = generateSeriesFromReturns("B", {0.1, 0.1});

    std::vector<PriceSeries> assets = {a, b};
    auto matrix = StatsEngine::covarianceMatrix(assets);

    REQUIRE(matrix[0][0] == Catch::Approx(matrix[1][1]));
    REQUIRE(matrix[0][1] == Catch::Approx(matrix[1][0]));
    REQUIRE(matrix[0][0] == Catch::Approx(matrix[0][1]));
}

TEST_CASE("Portfolio variance is computed correctly", "[portfolio][variance]")
{
    std::vector<std::vector<double>> covMatrix = {
        {0.01, 0.005},
        {0.005, 0.02}};

    std::vector<double> weights = {0.5, 0.5};
    double variance = StatsEngine::portfolioVariance(covMatrix, weights);

    REQUIRE(variance == Catch::Approx(0.01).epsilon(1e-6));
}

TEST_CASE("Expected portfolio return is computed correctly", "[portfolio][return]")
{
    std::vector<double> meanReturns = {0.01, 0.03};
    std::vector<double> weights = {0.5, 0.5};

    double result = StatsEngine::expectedPortfolioReturn(meanReturns, weights);

    REQUIRE(result == Catch::Approx(0.02).epsilon(1e-6));
}

TEST_CASE("Sharpe ratio is calculated correctly", "[sharpe]")
{
    double expectedReturn = 0.10;
    double variance = 0.04;
    double riskFree = 0.02;

    double result = StatsEngine::computeSharpeRatio(expectedReturn, variance, riskFree);

    REQUIRE(result == Catch::Approx(0.4).epsilon(1e-6));
}

TEST_CASE("Sortino ratio is computed correctly", "[sortino]")
{
    std::vector<double> returns = {0.01, 0.03, -0.02, -0.01, 0.04};
    double expectedReturn = 0.01;
    double riskFree = 0.0;

    double result = StatsEngine::computeSortinoRatio(expectedReturn, riskFree, returns);

    REQUIRE(result == Catch::Approx(0.632).epsilon(0.001));
}

TEST_CASE("Max drawdown is computed correctly", "[drawdown]")
{
    std::vector<double> cumulativeReturns = {1.0, 1.1, 0.95, 0.92, 1.05, 1.2, 1.0};
    double result = StatsEngine::computeMaxDrawdown(cumulativeReturns);

    REQUIRE(result == Catch::Approx(1.0 / 6.0).epsilon(0.00001));
}

TEST_CASE("Rolling volatility is computed correctly", "[rolling][volatility]")
{
    std::vector<double> returns = {1.0, 2.0, 3.0, 2.0, 1.0};
    size_t windowSize = 3;

    std::vector<double> result = StatsEngine::computeRollingVolatility(returns, windowSize);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == Catch::Approx(0.8165).epsilon(0.001));
    REQUIRE(result[1] == Catch::Approx(0.471).epsilon(0.001));
    REQUIRE(result[2] == Catch::Approx(0.8165).epsilon(0.001));
}

TEST_CASE("Rolling Sharpe ratio is computed correctly", "[rolling][sharpe]")
{
    std::vector<double> returns = {1.0, 2.0, 3.0, 2.0, 1.0};
    size_t windowSize = 3;
    double riskFreeRate = 0.0;

    std::vector<double> result = StatsEngine::computeRollingSharpe(returns, windowSize, riskFreeRate);

    REQUIRE(result.size() == 3);
    REQUIRE(result[0] == Catch::Approx(2.45).epsilon(0.01));
    REQUIRE(result[1] == Catch::Approx(4.94).epsilon(0.01));
    REQUIRE(result[2] == Catch::Approx(2.45).epsilon(0.01));
}

TEST_CASE("Rolling Sortino ratio computes values", "[rolling][sortino]")
{
    std::vector<double> returns = {0.01, -0.02, 0.03, -0.01, 0.02};
    size_t windowSize = 3;
    double riskFreeRate = 0.0;

    auto result = StatsEngine::computeRollingSortino(returns, windowSize, riskFreeRate);
    REQUIRE(result.size() == 3);
    REQUIRE(result[0] > 0);
}

TEST_CASE("SMA computes correctly", "[sma]")
{
    std::vector<double> data = {1, 2, 3, 4, 5};
    auto sma = StatsEngine::simpleMovingAverage(data, 3);
    REQUIRE(sma.size() == 3);
    REQUIRE(sma[0] == Catch::Approx(2.0));
    REQUIRE(sma[1] == Catch::Approx(3.0));
    REQUIRE(sma[2] == Catch::Approx(4.0));
}

TEST_CASE("EMA computes correctly", "[ema]")
{
    std::vector<double> data = {1, 2, 3, 4, 5};
    auto ema = StatsEngine::exponentialMovingAverage(data, 3);
    REQUIRE(ema.size() == data.size());
    REQUIRE(ema[0] == Catch::Approx(1.0));
    REQUIRE(ema.back() > ema[0]);
}

TEST_CASE("Correlation matrix is computed robustly", "[correlation]")
{
    std::vector<double> baseReturns = {0.1, -0.05, 0.2, -0.1};

    PriceSeries x = generateSeriesFromReturns("X", baseReturns);
    PriceSeries y = generateSeriesFromReturns("Y", baseReturns);
    PriceSeries z = generateInverseSeriesFromReturns("Z", baseReturns);

    std::vector<PriceSeries> assets = {x, y, z};
    auto corr = StatsEngine::computeCorrelationMatrix(assets);

    REQUIRE(corr.size() == 3);
    REQUIRE(corr[0].size() == 3);

    REQUIRE(corr[0][0] == Catch::Approx(1.0));
    REQUIRE(corr[1][1] == Catch::Approx(1.0));
    REQUIRE(corr[2][2] == Catch::Approx(1.0));

    REQUIRE(corr[0][1] == Catch::Approx(1.0).epsilon(0.01));
    REQUIRE(corr[0][2] == Catch::Approx(-1.0).epsilon(0.01));
}

TEST_CASE("Beta is computed correctly", "[beta]")
{
    auto benchmark = generateSeriesFromReturns("SPY", {0.01, 0.02, 0.03, 0.04});
    auto asset_same = generateSeriesFromReturns("SAME", {0.01, 0.02, 0.03, 0.04});
    auto asset_double = generateSeriesFromReturns("DOUBLE", {0.02, 0.04, 0.06, 0.08});

    double beta_same = StatsEngine::computeBeta(asset_same, benchmark);
    double beta_double = StatsEngine::computeBeta(asset_double, benchmark);

    REQUIRE(beta_same == Catch::Approx(1.0).epsilon(0.01));
    REQUIRE(beta_double == Catch::Approx(2.0).epsilon(0.01));
}

TEST_CASE("Alpha is computed correctly with manual beta", "[alpha]")
{
    // Assume we manually define mean portfolio return and mean market return
    PriceSeries portfolio = generateSeriesFromReturns("P", std::vector<double>{0.12, 0.12});
    PriceSeries market = generateSeriesFromReturns("M", std::vector<double>{0.10, 0.10});

    double riskFreeRate = 0.02; // 2% annualized

    // Manually specify beta
    double beta = 1.0;

    // Expected alpha using CAPM: α = Rp - [Rf + β * (Rm - Rf)]
    // α = 0.12 - [0.02 + 1.0 * (0.10 - 0.02)] = 0.12 - 0.10 = 0.02
    double expectedAlpha = 0.02;

    double computedAlpha = StatsEngine::computeAlpha(portfolio, market, riskFreeRate, beta);

    REQUIRE(computedAlpha == Catch::Approx(expectedAlpha).epsilon(1e-6));
}

TEST_CASE("Treynor Ratio: standard Computation", "[treynor]")
{
    double expectedReturn = 0.12;
    double riskFreeRate = 0.02;
    double beta = 1.25;

    double result = StatsEngine::computeTreynorRatio(expectedReturn, riskFreeRate, beta);
    REQUIRE(result == Catch::Approx(0.08).epsilon(0.01));
}

TEST_CASE("Treynor Ratio: zero beta returns 0", "[treynor]")
{
    double result = StatsEngine::computeTreynorRatio(0.12, 0.02, 0.0);
    REQUIRE(result == 0);
}

TEST_CASE("Treynor Ratio: negative beta handled", "[treynor]")
{
    double result = StatsEngine::computeTreynorRatio(0.12, 0.02, -1.0);
    REQUIRE(result == Catch::Approx(-0.10).epsilon(0.01));
}

TEST_CASE("Treynor Ratio: zero excess return", "[treynor]")
{
    double result = StatsEngine::computeTreynorRatio(0.02, 0.02, 1.0);
    REQUIRE(result == Catch::Approx(0.0).margin(1e-8));
}

TEST_CASE("Information Ratio: standard case", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {0.08, 0.10, 0.12, 0.09, 0.11};
    std::vector<double> benchmarkReturns = {0.05, 0.05, 0.05, 0.05, 0.05};

    double result = StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns);

    REQUIRE(result == Catch::Approx(3.54).epsilon(0.01));
}

TEST_CASE("Information Ratio: perfect match yields 0", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {0.05, 0.05, 0.05};
    std::vector<double> benchmarkReturns = {0.05, 0.05, 0.05};

    double result = StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns);

    REQUIRE(result == Catch::Approx(0.0).margin(1e-8));
}

TEST_CASE("Information Ratio: negative active returns", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {0.03, 0.02, 0.01};
    std::vector<double> benchmarkReturns = {0.05, 0.05, 0.05};

    double result = StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns);

    REQUIRE(result < 0.0);
}

TEST_CASE("Information Ratio: zero sttdev yields 0", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {0.06, 0.06, 0.06};
    std::vector<double> benchmarkReturns = {0.05, 0.05, 0.05};

    double result = StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns);

    REQUIRE(result == Catch::Approx(0.0).margin(1e-8));
}

TEST_CASE("Information Ratio: mismatched lengths throws ", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {0.08, 0.010, 0.12};
    std::vector<double> benchmarkReturns = {0.05, 0.05};

    REQUIRE_THROWS_AS(StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns), std::invalid_argument);
}

TEST_CASE("Information Ratio: empty input throws ", "[information_ratio]")
{
    std::vector<double> portfolioReturns = {};
    std::vector<double> benchmarkReturns = {};

    REQUIRE_THROWS_AS(StatsEngine::computeInformationRatio(portfolioReturns, benchmarkReturns), std::invalid_argument);
}

TEST_CASE("Calmar Ratio is computed correctly", "[calmar]")
{
    SECTION("Normal case")
    {
        double annualReturn = 0.3;
        double maxDrawdown = 0.1;

        double result = StatsEngine::computeCalmarRatio(annualReturn, maxDrawdown);
        REQUIRE(result == Catch::Approx(3.0).margin(0.0001));
    }

    SECTION("Zero drawdown returns infinity")
    {
        double annualReturn = 0.10;
        double maxDrawdown = 0.0;

        double result = StatsEngine::computeCalmarRatio(annualReturn, maxDrawdown);
        REQUIRE(std::isinf(result));
    }

    SECTION("Zero return yields zero Calmar Ratio")
    {
        double annualReturn = 0.00;
        double maxDrawdown = 0.2;

        double result = StatsEngine::computeCalmarRatio(annualReturn, maxDrawdown);
        REQUIRE(result == Catch::Approx(0.0));
    }
}


TEST_CASE("Omega Ratio is computed correctly", "[omega]") {
    SECTION("Balanced case") {
        std::vector<double> returns = {0.02, 0.04, -0.01, -0.02, 0.03};
        double threshold = 0.0;

        double result = StatsEngine::computeOmegaRatio(returns, threshold);
        REQUIRE(result == Catch::Approx((0.02+0.04+0.03) / (0.01+0.02)).epsilon(0.001));  // 0.09 / 0.03 = 3.0
    }

    SECTION("All returns above threshold") {
        std::vector<double> returns = {0.03, 0.04, 0.05};
        double threshold = 0.01;

        double result = StatsEngine::computeOmegaRatio(returns, threshold);
        REQUIRE(std::isinf(result));
    }

    SECTION("All returns below threshold") {
        std::vector<double> returns = {-0.02, -0.01, -0.03};
        double threshold = 0.0;

        double result = StatsEngine::computeOmegaRatio(returns, threshold);
        REQUIRE(result == Catch::Approx(0.0).margin(0.0001));
    }

    SECTION("Mixed returns, threshold at 0.01") {
        std::vector<double> returns = {0.015, 0.005, -0.005, 0.025};
        double threshold = 0.01;

        // Above: 0.005 (0.015 - 0.01) + 0.015 (0.025 - 0.01) = 0.02
        // Below: 0.005 (0.01 - 0.005) + 0.015 (0.01 - (-0.005)) = 0.02
        double result = StatsEngine::computeOmegaRatio(returns, threshold);
        REQUIRE(result == Catch::Approx(1.0).margin(0.0001));
    }
}

TEST_CASE("Computing Daily Return", "[daily_return]") {

}


TEST_CASE("Daily returns: increasing prices", "[daily_returns]") {
    PriceSeries ps("UP", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 110.0, 121.0});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 2);
    REQUIRE(returns[0] == Catch::Approx(0.10));
    REQUIRE(returns[1] == Catch::Approx(0.10));
}

TEST_CASE("Daily returns: decreasing prices", "[daily_returns]") {
    PriceSeries ps("DOWN", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 90.0, 81.0});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 2);
    REQUIRE(returns[0] == Catch::Approx(-0.10));
    REQUIRE(returns[1] == Catch::Approx(-0.10));
}

TEST_CASE("Daily returns: flat prices", "[daily_returns]") {
    PriceSeries ps("FLAT", {"2023-01-01", "2023-01-02", "2023-01-03"}, {100.0, 100.0, 100.0});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 2);
    REQUIRE(returns[0] == Catch::Approx(0.0));
    REQUIRE(returns[1] == Catch::Approx(0.0));
}

TEST_CASE("Daily returns: single price only", "[daily_returns]") {
    PriceSeries ps("SINGLE", {"2023-01-01"}, {100.0});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.empty());
}

TEST_CASE("Daily returns: empty series", "[daily_returns]") {
    PriceSeries ps("EMPTY", {}, {});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.empty());
}

TEST_CASE("Daily returns: large price jump", "[daily_returns]") {
    PriceSeries ps("JUMP", {"2023-01-01", "2023-01-02"}, {1.0, 1000.0});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 1);
    REQUIRE(returns[0] == Catch::Approx(999.0));
}

TEST_CASE("Daily returns: price drops to near zero", "[daily_returns]") {
    PriceSeries ps("DROP", {"2023-01-01", "2023-01-02"}, {1000.0, 0.01});
    auto returns = StatsEngine::computeDailyReturns(ps);

    REQUIRE(returns.size() == 1);
    REQUIRE(returns[0] == Catch::Approx(-0.99999).margin(1e-5));
}


TEST_CASE("Annualized Return - Normal case", "[annual_return]") {
    double totalReturn = 0.10; // 10%
    int numPeriods = 12;       // e.g., 12 months
    int periodsPerYear = 12;

    double result = StatsEngine::computeAnnualizedReturn(totalReturn, numPeriods, periodsPerYear);

    // Expected: (1 + 0.10)^(12/12) - 1 = 0.10
    REQUIRE(result == Catch::Approx(0.10).margin(0.0001));
}

TEST_CASE("Annualized Return - Compounded over multiple years", "[annual_return]") {
    double totalReturn = 0.21;
    int numPeriods = 24;       // 2 years monthly
    int periodsPerYear = 12;

    double result = StatsEngine::computeAnnualizedReturn(totalReturn, numPeriods, periodsPerYear);

    // Expected: (1 + 0.21)^(12/24) - 1 ≈ 0.10
    REQUIRE(result == Catch::Approx(0.10).margin(0.0001));
}

TEST_CASE("Annualized Return - Daily returns", "[annual_return]") {
    double totalReturn = 0.05;
    int numPeriods = 252;       // trading days
    int periodsPerYear = 252;

    double result = StatsEngine::computeAnnualizedReturn(totalReturn, numPeriods, periodsPerYear);

    REQUIRE(result == Catch::Approx(0.05).margin(0.0001));
}

TEST_CASE("Annualized Return - Zero total return", "[annual_return]") {
    double result = StatsEngine::computeAnnualizedReturn(0.0, 12, 12);
    REQUIRE(result == Catch::Approx(0.0).margin(0.0001));
}

TEST_CASE("Annualized Return - Negative return", "[annual_return]") {
    double result = StatsEngine::computeAnnualizedReturn(-0.10, 12, 12);
    REQUIRE(result == Catch::Approx(-0.10).margin(0.0001));
}

TEST_CASE("Annualized Return - Invalid inputs throw", "[annual_return]") {
    REQUIRE_THROWS_AS(StatsEngine::computeAnnualizedReturn(0.1, 0, 12), std::invalid_argument);
    REQUIRE_THROWS_AS(StatsEngine::computeAnnualizedReturn(0.1, 12, 0), std::invalid_argument);
    REQUIRE_THROWS_AS(StatsEngine::computeAnnualizedReturn(0.1, -1, 12), std::invalid_argument);
}