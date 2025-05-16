
# TradeIQ — Quant Research & Trading Dashboard

![CI](https://github.com/Ciaran-06/TradeIQ/actions/workflows/ci.yml/badge.svg)

> 🧠 **Quantitative research platform** for backtesting, analytics, and strategy simulation.  
> 🛠 Built in **modern C++17** with planned Python and React frontends. Powered by **Tiingo API** data.

---

## 🌟 Features

- 📈 **Data ingestion**: Fetch and cache historical data via Tiingo (JSON over HTTPS)
- 📊 **Statistics engine**: Portfolio analytics (returns, volatility, Sharpe, Sortino, drawdowns, etc.)
- 🧮 **Optimisation**: Markowitz mean-variance optimiser
- 🧪 **Strategy simulation**: Plug-and-play engine for weight-based strategies
- 🖥 **CLI interface**: Run analysis from terminal
- 🧬 **Python bindings**: (WIP) Integration via `pybind11` for notebooks/Streamlit
- 📊 **Web dashboard**: (WIP) React frontend for interactive results + charts

---

## 🔍 Use Case

Use TradeIQ to:

- Fetch and manage price series
- Backtest strategies with daily/rolling metrics
- Generate efficient frontiers and compare allocations
- Visualise drawdowns, correlations, and returns
- Export metrics for Python or reporting tools

---

## 📂 Project Structure

```
TradeIQ/
├── src/
│   ├── api/             # Tiingo HTTP client
│   ├── core/            # StatsEngine, strategy logic
│   ├── stats/           # Analytics: drawdowns, ratios, volatility
│   ├── utils/           # Helpers & math
│   └── cli/             # Terminal output (matrix printer, etc.)
├── tests/               # GTest suite
├── python/              # Pybind11 interface (planned)
├── web/                 # React dashboard (planned)
├── .env                 # API key storage (not committed)
├── main.cpp             # CLI entry point
├── CMakeLists.txt       # CMake build config
```

---

## 🚀 Getting Started

1. **Clone the repo**

```bash
git clone https://github.com/Ciaran-06/TradeIQ.git
cd TradeIQ
```

2. **Add API key**

```bash
echo "TIINGO_API_KEY=your_key_here" > .env
```

3. **Build the project**

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j
```

4. **Run**

```bash
./main_exec
```

---

## ✅ Run Tests

```bash
cd build
ctest --output-on-failure
```

Or run the test binary directly:

```bash
./tests
```

---

## 🧪 Dependencies

| Library            | Purpose             |
|--------------------|---------------------|
| [`cpr`](https://github.com/libcpr/cpr)           | HTTP client (for API calls)         |
| [`nlohmann/json`](https://github.com/nlohmann/json) | JSON parsing                        |
| [`dotenv-cpp`](https://github.com/laserpants/dotenv-cpp) | Load API keys from `.env`           |
| [`Google Test`](https://github.com/google/googletest)     | Unit testing framework              |
| [`CMake`](https://cmake.org/)                    | Build system                        |

---

## 🔧 Build Options

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake .. -DENABLE_PYTHON=ON          # optional pybind11 build
```

---

## 📊 Example Output

```
Fetching: AAPL, MSFT, JPM...

Returns:
  AAPL: 0.0123
  MSFT: 0.0098
  JPM : 0.0075

Covariance Matrix:
        AAPL     MSFT     JPM
AAPL    0.002    0.0012   0.0009
MSFT    0.0012   0.0021   0.001
JPM     0.0009   0.001    0.0022

Optimised Weights:
  AAPL:  40%
  MSFT:  30%
  JPM :  30%
```

---

## 🧠 Roadmap

- [x] Core StatsEngine (Sharpe, Sortino, Alpha, Drawdown)
- [x] CMake + CI Integration (GitHub Actions)
- [x] GTest migration from Catch2
- [ ] API mocking for test coverage
- [ ] Python bindings (via `pybind11`)
- [ ] React dashboard with charting
- [ ] Broker simulation (paper trade mode)

---

## 💡 Contributing

Pull requests welcome for:

- 📈 Financial metrics or portfolio models
- 🌐 Frontend dashboard (React + Plotly)
- 🧪 Test coverage improvements
- ⚙️ CI tools, linters, CMake tweaks

---

## 🛡 License

[MIT License](LICENSE)

---

### 👨‍💻 Built for learning. Extend for production.  
*Quant tools should be accessible, modular, and reproducible.*

