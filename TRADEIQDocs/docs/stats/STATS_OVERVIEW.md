# 📊 StatsEngine Function Overview

This document outlines all statistical and financial analysis functions available in the Stats module. Functions are grouped by category for clarity and maintainability.

---

## 📈 Returns

- `computeDailyReturns(const PriceSeries&)`  
  Computes day-over-day return values for a price series.

- `computeTotalReturn(const PriceSeries&)`  
  Calculates the total compounded return across a full series.

- `computeAnnualizedReturn(double totalReturn, int numPeriods, int periodsPerYear)`  
  Converts a total return into an annualized return using compounding.

---

## 📊 Risk & Volatility

- `portfolioVariance(const CovMatrix&, const Weights&)`  
  Computes variance of a portfolio using the covariance matrix and asset weights.

- `computeSharpeRatio(double expectedReturn, double volatility, double riskFreeRate)`  
  Measures excess return per unit of total risk.

- `computeSortinoRatio(double expectedReturn, double riskFreeRate, const Returns&)`  
  Like Sharpe but penalizes only downside volatility.

- `computeAnnualizedVolatility(const Returns&, int periodsPerYear)`  
  Converts standard deviation to annualized volatility.

- `computeRollingVolatility(const Returns&, size_t window)`  
  Rolling volatility across the series using a moving window.

- `computeRollingStandardDeviation(const Returns&, bool sample = false)`  
  Computes rolling standard deviation; sample mode optionally enabled.

---

## 🔁 Rolling Metrics

- `computeRollingSharpe(const Returns&, size_t window, double riskFreeRate)`  
  Computes rolling Sharpe ratio over a defined window.

- `computeRollingSortino(const Returns&, size_t window, double riskFreeRate)`  
  Computes rolling Sortino ratio.

---

## 📉 Drawdowns & Recovery

- `computeMaxDrawdown(const std::vector<double>& cumulativeReturns)`  
  Calculates the largest peak-to-trough decline.

- `computeAverageDrawdown(const std::vector<double>& cumulativeReturns)`  
  Mean drawdown across all drawdown events.

- `computeMaxRecoveryTime(const std::vector<double>& cumulativeReturns)`  
  Longest number of periods taken to recover from a drawdown.

---

## ⚖️ Ratios (Risk-Adjusted Returns)

- `computeAlpha(asset, benchmark, riskFreeRate)`  
  Computes alpha by inferring beta internally.

- `computeAlpha(portfolio, market, riskFreeRate, beta)`  
  Computes alpha with a manually supplied beta.

- `computeBeta(asset, benchmark)`  
  Measures volatility relative to the market.

- `computeTreynorRatio(expectedReturn, riskFreeRate, beta)`  
  Measures return per unit of systematic risk.

- `computeInformationRatio(portfolioReturns, benchmarkReturns)`  
  Ratio of active return to tracking error.

- `computeCalmarRatio(annualReturn, maxDrawdown)`  
  Return-to-drawdown ratio, especially useful for leveraged portfolios.

- `computeSterlingRatio(averageReturn, riskFreeRate, averageDrawdown)`  
  Adjusted return metric factoring drawdowns and risk-free rate.

- `computeOmegaRatio(const Returns&, threshold)`  
  Measures gains relative to losses above/below a given threshold.

---

## 🧮 Averages

- `simpleMovingAverage(const std::vector<double>&, size_t window)`  
  Computes SMA using a sliding window.

- `exponentialMovingAverage(const std::vector<double>&, size_t window)`  
  Computes EMA with exponentially decaying weights.

---

## 📈 Capture Ratios

- `computeUpsideCaptureRatio(portfolioReturns, benchmarkReturns)`  
  Measures portfolio performance during positive benchmark periods.

- `computeDownsideCaptureRatio(portfolioReturns, benchmarkReturns)`  
  Measures portfolio underperformance in negative benchmark periods.

---

## 🧠 Distribution Metrics

- `computeSkewness(const Returns&)`  
  Measures asymmetry of the return distribution.

- `computeKurtosis(const Returns&)`  
  Measures the "tailedness" of the distribution; returns excess kurtosis.

- `computeGainLossRatio(const Returns&)`  
  Ratio of sum of gains to sum of losses.

- `computeHitRatio(const Returns&)`  
  Proportion of periods with positive return.

---

## 🔗 Correlation & Covariance

- `covarianceMatrix(const std::vector<PriceSeries>&)`  
  Computes pairwise covariance matrix from multiple assets.

- `computeCorrelationMatrix(const std::vector<PriceSeries>&)`  
  Converts covariance matrix into correlation coefficients.

---

## 🧪 Utilities

- `exportToCSV(const std::string&, const std::vector<std::string>&, const std::vector<std::vector<double>>& data)`  
  Exports labeled tabular data to CSV format.

---

## ✅ Notes

- Most functions validate input and throw `std::invalid_argument` when assumptions are violated.
- Volatility functions assume **simple returns**, not log returns.
- Capture and ratio metrics are safe-guarded against divide-by-zero edge cases.

---

For usage examples, see the `tests/stats/` folder.
