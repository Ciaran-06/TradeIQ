#include "core/stats_engine.hpp"

#include <fstream>

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

double StatsEngine::meanReturns(const std::vector<double> &returns)
{
    double sum = 0.0;
    for (double r : returns)
    {
        sum += r;
    }
    return sum / returns.size();
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

std::vector<double> StatsEngine::computeRollingSharpe(const std::vector<double> &returns,
                                                      size_t windowSize,
                                                      double riskFreeRate)
{
    std::vector<double> rollingSharpe;

    if (returns.size() < windowSize || windowSize <= 1)
        return rollingSharpe;

    for (size_t i = windowSize - 1; i < returns.size(); ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < windowSize; ++j)
        {
            sum += returns[i - j];
        }
        double mean = sum / windowSize;

        double squaredDiff = 0.0;
        for (int j = 0; j < windowSize; ++j)
        {
            double diff = returns[i - j] - mean;
            squaredDiff += diff * diff;
        }
        double stddev = std::sqrt(squaredDiff / windowSize);

        double sharpe = 0.0;
        if (stddev != 0.0)
        {
            sharpe = (mean - riskFreeRate) / stddev;
        }

        rollingSharpe.push_back(sharpe);
    }

    return rollingSharpe;
}

std::vector<double> StatsEngine::computeRollingVolatility(const std::vector<double> &returns, size_t windowSize)
{
    std::vector<double> rollingVol;

    if (returns.size() < windowSize || windowSize <= 1)
        return rollingVol;

    for (size_t i = windowSize - 1; i < returns.size(); ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < windowSize; ++j)
            sum += returns[i - j];
        double mean = sum / windowSize;

        double squaredDiff = 0.0;
        for (int j = 0; j < windowSize; ++j)
        {
            double diff = returns[i - j] - mean;
            squaredDiff += diff * diff;
        }

        double stddev = std::sqrt(squaredDiff / windowSize);
        rollingVol.push_back(stddev);
    }

    return rollingVol;
}

std::vector<double> StatsEngine::computeRollingStandardDeviation(const std::vector<double> &returns, bool sample)
{
    if (returns.size() == 0)
        throw std::invalid_argument("Vector cannot be empty");
    std::vector<double> rollingStandardDeviation;
}

std::vector<std::vector<double>> StatsEngine::computeCorrelationMatrix(const std::vector<PriceSeries> &seriesList)
{
    size_t n = seriesList.size();
    std::vector<std::vector<double>> correlationMatrix(n, std::vector<double>(n, 0.0));

    // Step 1: Compute daily returns for each asset
    std::vector<std::vector<double>> allReturns;
    for (const auto &series : seriesList)
    {
        allReturns.push_back(StatsEngine::computeDailyReturns(series));
    }

    // Step 2: Compute mean of returns for each asset
    std::vector<double> means(n, 0.0);
    for (size_t i = 0; i < n; ++i)
    {
        const auto &r = allReturns[i];
        for (double val : r)
        {
            means[i] += val;
        }
        means[i] /= r.size();
    }

    // Step 3: Compute standard deviations
    std::vector<double> stdDevs(n, 0.0);
    for (size_t i = 0; i < n; ++i)
    {
        const auto &r = allReturns[i];
        double sumSq = 0.0;
        for (double val : r)
        {
            sumSq += (val - means[i]) * (val - means[i]);
        }
        stdDevs[i] = std::sqrt(sumSq / r.size());
    }

    // Step 4: Compute correlation coefficients
    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            const auto &r_i = allReturns[i];
            const auto &r_j = allReturns[j];
            size_t len = std::min(r_i.size(), r_j.size());

            double cov = 0.0;
            for (size_t k = 0; k < len; ++k)
            {
                cov += (r_i[k] - means[i]) * (r_j[k] - means[j]);
            }
            cov /= len;

            if (stdDevs[i] > 0.0 && stdDevs[j] > 0.0)
            {
                correlationMatrix[i][j] = cov / (stdDevs[i] * stdDevs[j]);
            }
            else
            {
                // Fallback: if no variation, define diagonal as 1, off-diagonal as 0
                correlationMatrix[i][j] = (i == j) ? 1.0 : 0.0;
            }
        }
    }

    return correlationMatrix;
}

std::vector<double> StatsEngine::computeRollingSortino(const std::vector<double> &returns, size_t windowSize, double riskFreeRate)
{
    std::vector<double> rollingSortino;

    if (returns.size() < windowSize || windowSize <= 1)
        return rollingSortino;

    for (size_t i = windowSize - 1; i < returns.size(); ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < windowSize; ++j)
        {
            sum += returns[i - j];
        }
        double mean = sum / windowSize;

        double downsideSumSq = 0.0;
        int downsideCount = 0;
        for (int j = 0; j < windowSize; ++j)
        {
            double diff = returns[i - j];
            if (diff < riskFreeRate)
            {
                downsideSumSq += (diff - riskFreeRate) * (diff - riskFreeRate);
                ++downsideCount;
            }
        }

        if (downsideCount > 0)
        {
            double downsideDev = std::sqrt(downsideSumSq / downsideCount);
            rollingSortino.push_back((mean - riskFreeRate) / downsideDev);
        }
        else
        {
            rollingSortino.push_back(0.0);
        }
    }

    return rollingSortino;
}

std::vector<double> StatsEngine::simpleMovingAverage(const std::vector<double> &data, size_t windowSize)
{
    std::vector<double> sma;
    if (data.size() < windowSize)
        return sma;

    double sum = 0.0;
    for (int i = 0; i < windowSize; ++i)
        sum += data[i];
    sma.push_back(sum / windowSize);

    for (size_t i = windowSize; i < data.size(); ++i)
    {
        sum += data[i] - data[i - windowSize];
        sma.push_back(sum / windowSize);
    }

    return sma;
}

std::vector<double> StatsEngine::exponentialMovingAverage(const std::vector<double> &data, size_t windowSize)
{
    std::vector<double> ema;
    if (data.empty() || windowSize <= 0)
        return ema;

    double alpha = 2.0 / (windowSize + 1.0);
    ema.push_back(data[0]);

    for (size_t i = 1; i < data.size(); ++i)
    {
        double nextEma = alpha * data[i] + (1 - alpha) * ema.back();
        ema.push_back(nextEma);
    }

    return ema;
}

void StatsEngine::exportToCSV(const std::string &filename, const std::vector<std::string> &headers, const std::vector<std::vector<double>> &data)
{
    std::ofstream out(filename);
    if (!out)
        throw std::runtime_error("Failed to open file: " + filename);

    for (size_t i = 0; i < headers.size(); ++i)
    {
        out << headers[i];
        if (i < headers.size() - 1)
            out << ",";
    }
    out << "\n";

    size_t rows = data[0].size();
    for (size_t r = 0; r < rows; ++r)
    {
        for (size_t c = 0; c < data.size(); ++c)
        {
            out << data[c][r];
            if (c < data.size() - 1)
                out << ",";
        }
        out << "\n";
    }
}

double StatsEngine::computeBeta(const PriceSeries &asset, const PriceSeries &benchmark)
{
    auto assetReturns = computeDailyReturns(asset);
    auto benchmarkReturns = computeDailyReturns(benchmark);
    size_t n = std::min(assetReturns.size(), benchmarkReturns.size());

    double assetMean = meanReturns(assetReturns);
    double benchmarkMean = meanReturns(benchmarkReturns);

    double cov = 0.0;
    double var = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        cov += (assetReturns[i] - assetMean) * (benchmarkReturns[i] - benchmarkMean);
        var += (benchmarkReturns[i] - benchmarkMean) * (benchmarkReturns[i] - benchmarkMean);
    }

    if (var == 0.0)
        return 0.0;
    return cov / var;
}

double StatsEngine::computeAlpha(const PriceSeries &asset, const PriceSeries &benchmark, double riskFreeRate)
{
    auto assetReturns = computeDailyReturns(asset);
    auto benchmarkReturns = computeDailyReturns(benchmark);
    double beta = computeBeta(asset, benchmark);

    double assetMean = meanReturns(assetReturns);
    double benchmarkMean = meanReturns(benchmarkReturns);

    return assetMean - (riskFreeRate + beta * (benchmarkMean - riskFreeRate));
}

double StatsEngine::computeAlpha(const PriceSeries &portfolio,
                                 const PriceSeries &market,
                                 double riskFreeRate,
                                 double beta)
{
    double rp = meanReturns(computeDailyReturns(portfolio));
    double rm = meanReturns(computeDailyReturns(market));

    return rp - (riskFreeRate + beta * (rm - riskFreeRate));
}

double StatsEngine::computeTreynorRatio(double expectedReturns, double riskFreeRate, double portfolioBeta)
{
    if (portfolioBeta == 0)
    {
        printf("Portfolio Beta cannot be zero for Treynor Ratio");
        return 0.0;
    }
    else
    {
        return (expectedReturns - riskFreeRate) / portfolioBeta;
    }
}

double StatsEngine::computeInformationRatio(std::vector<double> &portfolioReturns, std::vector<double> benchmarkReturns)
{
    if (portfolioReturns.size() != benchmarkReturns.size())
    {
        throw std::invalid_argument("Portfolio Returns and Benchmark Returns are different lenghts");
    }
    if (portfolioReturns.size() == 0 || benchmarkReturns.size() == 0)
    {
        throw std::invalid_argument("Inputs can't be empty!");
    }
    std::vector<double> activeReturns;
    double meanActiveReturns = 0.0;
    double squaredDiffSum = 0.0;

    for (size_t i = 0; i < portfolioReturns.size(); ++i)
    {
        activeReturns.push_back(portfolioReturns[i] - benchmarkReturns[i]);
    }

    for (double r : activeReturns)
    {
        meanActiveReturns += r;
    }

    meanActiveReturns /= activeReturns.size();

    for (double r : activeReturns)
    {
        squaredDiffSum += (r - meanActiveReturns) * (r - meanActiveReturns);
    }

    double stddev = std::sqrt(squaredDiffSum / activeReturns.size());

    if (stddev != 0)
    {
        return (meanActiveReturns / stddev);
    }
    else
    {
        return 0;
    }
}

double StatsEngine::computeCalmarRatio(double annualReturn, double maxDrawdown)
{
    if (maxDrawdown == 0.0)
    {
        return std::numeric_limits<double>::infinity(); // no drawdown = infinite risk-adjusted return
    }
    else
    {
        return annualReturn / maxDrawdown;
    }
}

double StatsEngine::computeOmegaRatio(const std::vector<double> &returns, double threshold)
{
    double sumAbove = 0.0;
    double sumBelow = 0.0;

    for (double r : returns)
    {
        if (r > threshold)
        {
            sumAbove += (r - threshold);
        }
        else if (r < threshold)
        {
            sumBelow += (threshold - r);
        }
    }

    if (sumBelow == 0)
        return std::numeric_limits<double>::infinity();
    return sumAbove / sumBelow;
}

double StatsEngine::computeTotalReturn(const PriceSeries &series)
{
    const auto &prices = series.getPrices();

    double first = prices.front();
    double last = prices.back();
    if (prices.empty())
        return 0.0;
    if (first == 0.0)
        return std::numeric_limits<double>::infinity();
    return (last / first) - 1;
}

double StatsEngine::computeAnnualizedReturn(double totalReturn, int numPeriods, int periodsPerYear)
{
    if (numPeriods <= 0 || periodsPerYear <= 0)
        throw std::invalid_argument("Inputs are not valid");

    double growthFactor = 1 + totalReturn;

    return pow(growthFactor, static_cast<double>(periodsPerYear) / numPeriods) - 1.0;
}

double StatsEngine::computeAnnualizedVolatility(const std::vector<double> &returns, int periodsPerYear)
{
    if (returns.empty())
        throw std::invalid_argument("Returns vector cannot be empty.");
    if (periodsPerYear <= 0)
        throw std::invalid_argument("Periods per year must be positive.");

    double stddev = MathUtils::standardDeviation(returns, true);
    return stddev * std::sqrt(static_cast<double>(periodsPerYear));
}

double StatsEngine::computeAverageDrawdown(const std::vector<double> &cumulativeReturns)
{
    if (cumulativeReturns.empty())
        return 0.0;

    double peak = cumulativeReturns[0];
    double trough = cumulativeReturns[0];
    bool inDrawdown = false;
    std::vector<double> drawdowns;

    for (size_t i = 1; i < cumulativeReturns.size(); ++i)
    {
        double value = cumulativeReturns[i];

        if (value >= peak)
        {
            if (inDrawdown && trough < peak)
            {
                double drawdown = (peak - trough) / peak;
                drawdowns.push_back(drawdown);
            }
            peak = value;
            trough = value;
            inDrawdown = false;
        }
        else
        {
            inDrawdown = true;
            trough = std::min(trough, value);
        }
    }

    // Edge case: still in drawdown at end
    if (inDrawdown && trough < peak)
    {
        double drawdown = (peak - trough) / peak;
        drawdowns.push_back(drawdown);
    }

    if (drawdowns.empty())
        return 0.0;

    double sum = std::accumulate(drawdowns.begin(), drawdowns.end(), 0.0);
    return sum / drawdowns.size();
}

double StatsEngine::computeSterlingRatio(double averageReturn, double riskFreeRate, double averageDrawdown)
{
    if (averageDrawdown == 0)
        throw std::invalid_argument("Average drawdown can't be 0");

    return (averageReturn - riskFreeRate) / averageDrawdown;
}

double StatsEngine::computeVolatilitySkew(const std::vector<double> &returns)
{
    std::vector<double> positiveReturns;
    std::vector<double> negativeReturns;

    for (double r : returns)
    {
        if (r >= 0.0)
        {
            positiveReturns.push_back(r);
        }
        else
        {
            negativeReturns.push_back(r);
        }
    }

    if (positiveReturns.empty())
        throw std::invalid_argument("No positive returns — cannot compute positive standard deviation.");

    if (negativeReturns.empty())
        throw std::invalid_argument("No negative returns — cannot compute volatility skew.");

    double stddevPositive = MathUtils::standardDeviation(positiveReturns, true);
    double stddevNegative = MathUtils::standardDeviation(negativeReturns, true);

    if (stddevPositive == 0.0)
        throw std::invalid_argument("Standard deviation of positive returns is zero.");

    return stddevNegative / stddevPositive;
}

double StatsEngine::computeUpsideCaptureRatio(const std::vector<double> &portfolioReturns,
                                              const std::vector<double> &benchmarkReturns)
{
    if (portfolioReturns.size() != benchmarkReturns.size())
    {
        throw std::invalid_argument("Mismatched input sizes");
    }

    std::vector<double> portUpside;
    std::vector<double> benchUpside;

    for (size_t i = 0; i < benchmarkReturns.size(); ++i)
    {
        if (benchmarkReturns[i] > 0)
        {
            portUpside.push_back(portfolioReturns[i]);
            benchUpside.push_back(benchmarkReturns[i]);
        }
    }

    if (benchUpside.empty())
        return 0.0;

    double avgPort = MathUtils::mean(portUpside);
    double avgBench = MathUtils::mean(benchUpside);

    return avgBench == 0.0 ? 0.0 : avgPort / avgBench;
}

double StatsEngine::computeDownsideCaptureRatio(const std::vector<double> &portfolioReturns,
                                                const std::vector<double> &benchmarkReturns)
{
    if (portfolioReturns.size() != benchmarkReturns.size())
    {
        throw std::invalid_argument("Mismatched input sizes");
    }

    std::vector<double> portDownside;
    std::vector<double> benchDownside;

    for (size_t i = 0; i < benchmarkReturns.size(); ++i)
    {
        if (benchmarkReturns[i] < 0)
        {
            portDownside.push_back(portfolioReturns[i]);
            benchDownside.push_back(benchmarkReturns[i]);
        }
    }

    if (benchDownside.empty())
        return 0.0;

    double avgPort = MathUtils::mean(portDownside);
    double avgBench = MathUtils::mean(benchDownside);

    return avgBench == 0.0 ? 0.0 : avgPort / avgBench;
}

double StatsEngine::computeSkewness(const std::vector<double> &returns)
{
    if (std::adjacent_find(returns.begin(), returns.end(), std::not_equal_to<>()) == returns.end())
    {
        throw std::invalid_argument("Elements in Vector must be unique, Skewness of Non-Unique Vector is undefined(division by 0)");
    }
    if (returns.size() < 3)
        throw std::invalid_argument("Need at least 3 data points for skewness");
    double mean = MathUtils::mean(returns);
    double stddev = MathUtils::standardDeviation(returns);
    double sum = 0.0;
    for (double r : returns)
        sum += std::pow(r - mean, 3);
    return (sum / returns.size()) / std::pow(stddev, 3);
}

double StatsEngine::computeKurtosis(const std::vector<double> &returns)
{
    if (returns.size() < 4)
        throw std::invalid_argument("Need at least 4 data points for kurtosis");
    double mean = MathUtils::mean(returns);
    double stddev = MathUtils::standardDeviation(returns);
    double sum = 0.0;
    for (double r : returns)
        sum += std::pow(r - mean, 4);
    return (sum / returns.size()) / std::pow(stddev, 4) - 3.0;
}

double StatsEngine::computeGainLossRatio(const std::vector<double> &returns)
{
    if (returns.size() == 0)
        throw std::invalid_argument("Vector cannot be empty!");
    double gainSum = 0.0, lossSum = 0.0;
    for (double r : returns)
    {
        if (r > 0)
            gainSum += r;
        else
            lossSum -= r;
    }
    if (lossSum == 0.0)
        return std::numeric_limits<double>::infinity();
    return gainSum / lossSum;
}

double StatsEngine::computeHitRatio(const std::vector<double> &returns)
{
    if (returns.empty())
        throw std::invalid_argument("Vector cannot be empty!");
    size_t hits = 0;
    for (double r : returns)
        if (r > 0)
            ++hits;
    return static_cast<double>(hits) / returns.size();
}

int StatsEngine::computeMaxRecoveryTime(const std::vector<double> &cumulativeReturns)
{
    if (cumulativeReturns.empty()) return 0;

    double peak = cumulativeReturns[0];
    int peakIndex = 0;
    int maxRecoveryTime = 0;
    bool inDrawdown = false;

    for (size_t i = 1; i < cumulativeReturns.size(); ++i)
    {
        double value = cumulativeReturns[i];

        if (value > peak)
        {
            if (inDrawdown)
            {
                int recoveryTime = static_cast<int>(i - peakIndex);
                maxRecoveryTime = std::max(maxRecoveryTime, recoveryTime);
                inDrawdown = false;
            }
            peak = value;
            peakIndex = static_cast<int>(i);
        }
        else if (value < peak)
        {
            if (!inDrawdown)
            {
                inDrawdown = true;
            }
        }
    }

    // If still in drawdown at the end, count recovery time up to the last point
    if (inDrawdown)
    {
        int recoveryTime = static_cast<int>(cumulativeReturns.size()) - peakIndex;
        maxRecoveryTime = std::max(maxRecoveryTime, recoveryTime);
    }

    return maxRecoveryTime;
}
