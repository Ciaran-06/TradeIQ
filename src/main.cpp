#include <iostream>
#include <vector>
#include <algorithm>

#include "./api/tiingo_client.hpp"
#include "./core/price_series.hpp"
#include "./cli/matrix_printer.hpp"

#include "./stats/returns.hpp"
#include "./stats/ratios.hpp"
#include "./stats/volatility.hpp"
#include "./stats/drawdowns.hpp"
#include "./stats/correlation.hpp"

int main()
{
    TiingoClient client;

    std::string ticker = "AAPL";
    std::string startDate = "2023-01-01";
    std::string endDate = "2023-12-31";

    std::cout << "Fetching " << ticker << " data...\n";
    PriceSeries series = client.fetchDailyPrices(ticker, startDate, endDate);

    std::vector<double> returns = Stats::Returns::computeDailyReturns(series);
    double mean = Stats::Returns::meanReturns(returns);

    auto cov = Stats::Correlation::computeCorrelationMatrix({series});
    std::vector<double> weights = {1.0};
    double variance = Stats::Ratios::computePortfolioVariance(cov, weights);

    double sharpe = Stats::Ratios::computeSharpeRatio(mean, variance, 0.01);
    double sortino = Stats::Ratios::computeSortinoRatio(mean, 0.01, returns);

    std::vector<double> cumReturns;
    double cum = 1.0;
    for (double r : returns) {
        cum *= (1.0 + r);
        cumReturns.push_back(cum);
    }

    double mdd = Stats::Drawdowns::computeMaxDrawdown(cumReturns);

    std::cout << "\n📈 Stats for " << ticker << " (" << startDate << " to " << endDate << ")\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Mean Return      : " << mean << "\n";
    std::cout << "Variance         : " << variance << "\n";
    std::cout << "Sharpe Ratio     : " << sharpe << "\n";
    std::cout << "Sortino Ratio    : " << sortino << "\n";
    std::cout << "Max Drawdown     : " << mdd << "\n";

    std::cout << "\n📊 Covariance Matrix:\n";
    MatrixPrinter::print(cov);

    std::cout << "\n📉 Rolling Volatility (20d):\n";
    auto rollVol = Stats::Volatility::computeRollingVolatility(returns, 20);
    for (size_t i = 0; i < std::min(rollVol.size(), size_t(10)); ++i)
        std::cout << "Day " << i + 20 << ": " << rollVol[i] << "\n";

    std::cout << "\n📊 Rolling Sharpe (20d):\n";
    auto rollSharpe = Stats::Volatility::computeRollingSharpe(returns, 20, 0.01);
    for (size_t i = 0; i < std::min(rollSharpe.size(), size_t(10)); ++i)
        std::cout << "Day " << i + 20 << ": " << rollSharpe[i] << "\n";

    return 0;
}
