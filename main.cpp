#include "src/api/tiingo_client.hpp"
#include <iostream>

int main() {
    try {
        TiingoClient client;
        auto series = client.fetchDailyPrices("AAPL", "2023-01-01", "2023-06-01");

        std::cout << "Loaded " << series.getPrices().size() << " prices for " << series.getTicker() << "\n";

        auto returns = series.getDailyReturns();
        std::cout << "First 5 returns:\n";
        for (size_t i = 0; i < std::min<size_t>(5, returns.size()); ++i) {
            std::cout << returns[i] << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
