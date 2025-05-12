#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

#include "./api/tiingo_client.hpp"
#include "./core/stats_engine.hpp"
#include "./core/price_series.hpp"
#include "./cli/matrix_printer.hpp"  // <-- Include MatrixPrinter

int main()
{
    TiingoClient client;

    std::string ticker = "AAPL";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";

    std::cout << "Fetching " << ticker << " data...\n";
    PriceSeries series = client.fetchDailyPrices(ticker, startDate, endDate);

    std::vector<double> returns = StatsEngine::computeDailyReturns(series);
    double mean = StatsEngine::meanReturns(returns);

    auto cov = StatsEngine::covarianceMatrix({series});
    std::vector<double> weights = {1.0};
    double variance = StatsEngine::portfolioVariance(cov, weights);

    double sharpe = StatsEngine::computeSharpeRatio(mean, variance, 0.01);
    double sortino = StatsEngine::computeSortinoRatio(mean, 0.01, returns);

    std::vector<double> cumReturns;
    double cum = 1.0;
    for (double r : returns)
    {
        cum *= (1.0 + r);
        cumReturns.push_back(cum);
    }

    double mdd = StatsEngine::computeMaxDrawdown(cumReturns);

    std::cout << "\n📈 Stats for " << ticker << " (" << startDate << " to " << endDate << ")\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Mean Return      : " << mean << "\n";
    std::cout << "Variance         : " << variance << "\n";
    std::cout << "Sharpe Ratio     : " << sharpe << "\n";
    std::cout << "Sortino Ratio    : " << sortino << "\n";
    std::cout << "Max Drawdown     : " << mdd << "\n";

    std::cout << "\n📊 Covariance Matrix:\n";
    MatrixPrinter::print(cov);  // <-- MatrixPrinter in action

    std::cout << "\n📉 Rolling Volatility (20d):\n";
    auto rollVol = StatsEngine::computeRollingVolatility(returns, 20);
    for (size_t i = 0; i < std::min(rollVol.size(), size_t(10)); ++i)
        std::cout << "Day " << i + 20 << ": " << rollVol[i] << "\n";

    std::cout << "\n📊 Rolling Sharpe (20d):\n";
    auto rollSharpe = StatsEngine::computeRollingSharpe(returns, 20, 0.01);
    for (size_t i = 0; i < std::min(rollSharpe.size(), size_t(10)); ++i)
        std::cout << "Day " << i + 20 << ": " << rollSharpe[i] << "\n";

    return 0;
}
