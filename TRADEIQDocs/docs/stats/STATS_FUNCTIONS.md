# StatsEngine Function Overview

This file documents all available statistical functions grouped by category.

---

## 📈 Returns

- `computeDailyReturns(series)` – Computes daily log or simple returns.
- `computeTotalReturn(series)` – Calculates the overall return.
- `computeAnnualizedReturn(totalReturn, numPeriods, periodsPerYear)` – Annualized compound growth rate.

---

## 📊 Risk & Volatility

- `portfolioVariance(covMatrix, weights)` – Portfolio variance via matrix math.
- `computeSharpeRatio(expectedReturn, volatility, riskFreeRate)`
- `computeSortinoRatio(expectedReturn, riskFreeRate, returns)`
- `computeRollingVolatility(returns, window)`
- `computeAnnualizedVolatility(returns, periodsPerYear)`

---

## 📉 Drawdowns

- `computeMaxDrawdown(cumulativeReturns)`
- `computeAverageDrawdown(cumulativeReturns)`
- `computeMaxRecoveryTime(cumulativeReturns)`

---

## 📚 Ratios

- `computeAlpha(...)` – Two overloads, with or without beta input.
- `computeBeta(asset, benchmark)`
- `computeTreynorRatio(expectedReturn, riskFree, beta)`
- `computeInformationRatio(portfolioReturns, benchmarkReturns)`
- `computeCalmarRatio(annualReturn, maxDrawdown)`
- `computeSterlingRatio(avgReturn, riskFree, avgDrawdown)`
- `computeOmegaRatio(returns, threshold)`

---

## 🔁 Rolling Metrics

- `computeRollingSharpe(...)`
- `computeRollingSortino(...)`
- `computeRollingStandardDeviation(...)`

---

## 🧠 Distribution Metrics

- `computeSkewness(returns)`
- `computeKurtosis(returns)`
- `computeGainLossRatio(returns)`
- `computeHitRatio(returns)`

---

## 📈 Capture Metrics

- `computeUpsideCaptureRatio(portfolio, benchmark)`
- `computeDownsideCaptureRatio(portfolio, benchmark)`

---

## 🧮 Moving Averages

- `simpleMovingAverage(values, window)`
- `exponentialMovingAverage(values, window)`

---

## 🧪 Utilities

- `exportToCSV(filename, headers, data)`

---

For detailed usage and testing, see `tests/stats/`.
