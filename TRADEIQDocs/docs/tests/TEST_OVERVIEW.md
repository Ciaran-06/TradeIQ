# 🧪 Test Coverage Overview

This page summarizes the scope of automated tests written for the TradeIQ platform.

## ✅ Testing Framework

- **Framework**: Google Test (GTest)
- **Coverage**: All core components of `StatsEngine`, including edge cases and invalid input handling.

## 📂 Test Groups

| Category         | Functions Covered |
|------------------|-------------------|
| 📈 Returns       | `computeDailyReturns`, `computeTotalReturn`, `computeAnnualizedReturn` |
| 📊 Volatility    | `computeAnnualizedVolatility`, `computeRollingVolatility`, `computeRollingStandardDeviation` |
| ⚖️ Ratios        | Sharpe, Sortino, Treynor, Calmar, Information, Omega, Sterling |
| 📉 Drawdowns     | `computeMaxDrawdown`, `computeAverageDrawdown`, `computeMaxRecoveryTime` |
| 🔁 Rolling       | Rolling Sharpe / Sortino |
| 🔗 Correlation   | Covariance & correlation matrix functions |
| 📊 Distribution  | Skewness, Kurtosis, Gain/Loss Ratio, Hit Ratio |
| 📈 Capture       | Upside and Downside Capture Ratios |
| 🧰 Utilities     | `exportToCSV` and internal helpers |

## 🛠️ Notes

- Edge case inputs (e.g. zero stddev, empty vectors, mismatched lengths) are tested and throw exceptions as expected.
- Tests are located under the `tests/stats/` directory and split by category.
- Future test coverage will extend to API/CLI/GUI layers as implemented.

---

### 📌 See Also

- [Stats Engine Overview](../stats/STATS_OVERVIEW.md)
- [GitHub: Full Test Suite](https://github.com/Ciaran-06/TradeIQ/tree/main/tests)
