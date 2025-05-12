#include "matrix_printer.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

void MatrixPrinter::print(const std::map<std::string, PriceSeries>& data) {
    std::cout << "\n=== Price Matrix ===\n";

    // Collect all dates
    std::vector<std::string> dates;
    if (!data.empty()) {
        dates = data.begin()->second.getDates();
    }

    // Header row
    std::cout << std::setw(12) << "Date";
    for (const auto& [ticker, _] : data) {
        std::cout << std::setw(10) << ticker;
    }
    std::cout << "\n";

    // Data rows
    for (size_t i = 0; i < dates.size(); ++i) {
        std::cout << std::setw(12) << dates[i];
        for (const auto& [_, series] : data) {
            const auto& prices = series.getPrices();
            if (i < prices.size()) {
                std::cout << std::setw(10) << std::fixed << std::setprecision(2) << prices[i];
            } else {
                std::cout << std::setw(10) << "N/A";
            }
        }
        std::cout << "\n";
    }
}
