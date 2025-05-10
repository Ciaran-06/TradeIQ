# Mean-Variance-Opt — Quant Research & Trading Dashboard

> **Quant dashboard for research, analytics, and strategy development**
> Built in C++17 with Python & React interfaces, powered by Tiingo data.

## 🌟 Features

* **Data ingestion:** fetch & cache historical stock data via Tiingo API
* **Statistics engine:** compute daily returns, covariance matrix, portfolio variance
* **Optimisation:** mean-variance portfolio optimiser (Markowitz)
* **Strategy builder:** simulate strategies with custom weight inputs
* **Web dashboard:** real-time results, visualisation, backtest interface
* **Python interface:** plug into Jupyter or Streamlit

---

## 🔍 Use Case

Build and test portfolio strategies with:

* Historical price data
* Risk-adjusted return metrics
* Covariance heatmaps
* Efficient frontier plots
* Trading logic simulator

---

## 📂 Project Structure

```
Mean-Variance-Opt/
├── src/
│   ├── api/alphavantage.h/.cpp      # data fetching (Tiingo, switchable)
│   ├── core/                        # stats + optimisation engine
│   ├── utils/                       # helper functions
├── tests/                           # Catch2 test suite
├── web/                             # React dashboard (WIP)
├── python/                          # pybind11 interface
├── .env                             # store API_KEY
├── main.cpp                         # CLI entry point
├── CMakeLists.txt                   # build config
```

---

## 🚀 Getting Started

1. Clone the repo:

```bash
git clone https://github.com/yourname/Mean-Variance-Opt.git
cd Mean-Variance-Opt
```

2. Add your `.env` file:

```bash
echo "API_KEY=your_tiingo_key" > .env
```

3. Build project:

```bash
mkdir build && cd build
cmake .. && make -j
```

4. Run main:

```bash
./main
```

---

## 🧪 Dependencies

* [Tiingo API](https://api.tiingo.com/)
* [cpr](https://github.com/libcpr/cpr) - HTTP
* [nlohmann/json](https://github.com/nlohmann/json) - JSON
* [Catch2](https://github.com/catchorg/Catch2) - testing
* [dotenv-cpp](https://github.com/laserpants/dotenv-cpp) - env vars

---

## 🔧 Build Options

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

Optional:

```bash
cmake -DENABLE_PYTHON=ON ..
```

---

## 🎯 Example Output

```
Fetching AAPL...
  Date: 2023-05-10, Price: 165.32
...
Covariance Matrix:
AAPL  MSFT  JPM
0.002 0.001 0.0009
...
Optimised Weights:
AAPL: 0.4, MSFT: 0.3, JPM: 0.3
```

---

## 💪 Contributing

Open to PRs on:

* Front-end dashboard (React/Plotly)
* Broker integration
* Backtesting engine

---

## 📅 License

MIT License.

---

*Built for learning. Extend for real.*
