#include "stats_engine.hpp"

std::vector<double> StatsEngine::computeDailyReturns(const PriceSeries &series)
{
    const auto &prices = series.getPrices();
    std::vector<double> returns;

    for (size_t i = 1; i < prices.size(); ++i)
    {
        double ret = (prices[i] - prices[i - 1]) / prices[i - 1];
        returns.push_back(ret);
    }

    return returns;
}

std::vector<double> StatsEngine::meanReturns(const std::vector<PriceSeries> &seriesList)
{
    std::vector<double> means;

    for (const auto &series : seriesList)
    {
        auto returns = computeDailyReturns(series);
        if (returns.empty())
        {
            means.push_back(0.0); // or throw if preferred
            continue;
        }

        double sum = 0.0;
        for (double r : returns)
            sum += r;
        means.push_back(sum / static_cast<double>(returns.size()));
    }

    return means;
}

std::vector<std::vector<double>> StatsEngine::covarianceMatrix(const std::vector<PriceSeries> &seriesList)
{
    const size_t n = seriesList.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 0.0));

    // Precompute returns for all assets
    std::vector<std::vector<double>> allReturns;
    for (const auto &s : seriesList)
    {
        allReturns.push_back(computeDailyReturns(s));
    }

    // Compute means
    std::vector<double> means;
    for (const auto &r : allReturns)
    {
        double sum = 0.0;
        for (double val : r)
            sum += val;
        means.push_back(sum / r.size());
    }

    // Build covariance matrix
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            double sum = 0.0;
            size_t T = std::min(allReturns[i].size(), allReturns[j].size());
            for (size_t t = 0; t < T; ++t)
            {
                sum += (allReturns[i][t] - means[i]) * (allReturns[j][t] - means[j]);
            }
            matrix[i][j] = sum / static_cast<double>(T - 1);
        }
    }

    return matrix;
}

std::vector<std::vector<double>> covarianceMatrix(const std::vector<PriceSeries> &seriesList)
{

    std::vector<std::vector<double>> allReturns;
    std::vector<double> means;

    for (const auto &series : seriesList)
    {
        std::vector<double> returns = StatsEngine::computeDailyReturns(series);
        allReturns.push_back(returns);
    }

    for (const auto &returns : allReturns)
    {
        double sum = 0.0;
        for (double r : returns)
        {
            sum += r;
        }

        double mean = sum / returns.size();
        means.push_back(mean);
    }

    size_t n = allReturns.size();
    std::vector<std::vector<double>> covMatrix(n, std::vector<double>(n, 0.0));

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            const auto &xi = allReturns[i];
            const auto &xj = allReturns[j];
            double mean_i = means[i];
            double mean_j = means[j];

            double sum = 0.0;
            size_t T = std::min(xi.size(), xj.size()); // ensure they’re same length
            for (size_t t = 0; t < T; ++t)
            {
                sum += (xi[t] - mean_i) * (xj[t] - mean_j);
            }
            covMatrix[i][j] = sum / (T - 1); // sample covariance
        }
    }
    return covMatrix;
}

double StatsEngine::portfolioVariance(
    const std::vector<std::vector<double>> &covMatrix,
    const std::vector<double> &weights)
{
    double variance = 0.0;
    size_t n = weights.size();
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            variance += weights[i] * covMatrix[i][j] * weights[j];
        }
    }
    return variance;
}

double StatsEngine::expectedPortfolioReturn(
    const std::vector<double> &meanReturns,
    const std::vector<double> &weights)
{
    double expectedReturn = 0.0;
    for (size_t i = 0; i < weights.size(); ++i)
    {
        expectedReturn += weights[i] * meanReturns[i];
    }
    return expectedReturn;
}

double StatsEngine::computeSharpeRatio(
    double expectedPortfolioReturn,
    double portfolioVariance,
    double riskFreeRate)
{

    return (expectedPortfolioReturn - riskFreeRate) / sqrt(portfolioVariance);
}

double StatsEngine::computeSortinoRatio(
    double expectedPortfolioReturn,
    double riskFreeReturn,
    const std::vector<double> &portfolioReturns)
{
    double sumSquared = 0.0;
    size_t count = 0;

    for (double r : portfolioReturns)
    {
        if (r < riskFreeReturn)
        {
            sumSquared += (r - riskFreeReturn) * (r - riskFreeReturn);
            ++count;
        }
    }

    if (count == 0)
    {
        return 0.0; // No downside deviation — handle gracefully
    }

    double downsideDeviation = std::sqrt(sumSquared / count);
    return (expectedPortfolioReturn - riskFreeReturn) / downsideDeviation;
}

double StatsEngine::computeMaxDrawdown(const std::vector<double> &cumulativeReturns)
{
    double runningPeak = 0.0;
    double maxDownturn = 0.0;

    for (const auto &r : cumulativeReturns)
    {
        if (r > runningPeak)
        {
            runningPeak = r;
        }
        else
        {
            double currentDrawdown = (runningPeak - r) / runningPeak;
            if (currentDrawdown > maxDownturn)
            {
                maxDownturn = currentDrawdown;
            }
        }
    }

    return maxDownturn;
}
