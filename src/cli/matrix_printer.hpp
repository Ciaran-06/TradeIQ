#pragma once

#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "../core/price_series.hpp"

class MatrixPrinter {
public:
    static void print(const std::map<std::string, PriceSeries>& data);

    static void print(const std::vector<std::vector<double>>& matrix) {
        for (const auto& row : matrix) {
            for (double val : row) {
                std::cout << std::setw(10) << std::fixed << std::setprecision(4) << val << " ";
            }
            std::cout << "\n";
        }
    }
};