#pragma once

#include <string>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>
#include "../src/core/price_series.hpp"

// --- 1. Generate PriceSeries from exact returns ---
inline PriceSeries generateSeriesFromReturns(const std::string& ticker,
                                             const std::vector<double>& returns,
                                             double initialPrice = 1.0) {
    std::vector<double> prices = {initialPrice};
    for (double r : returns) {
        prices.push_back(prices.back() * (1.0 + r));
    }
    std::vector<std::string> dates(prices.size(), "");
    return PriceSeries(ticker, dates, prices);
}

// --- 2. Generate inverse (anti-correlated) PriceSeries from returns ---
inline PriceSeries generateInverseSeriesFromReturns(const std::string& ticker,
                                                    const std::vector<double>& originalReturns,
                                                    double initialPrice = 1.0) {
    std::vector<double> negated;
    for (double r : originalReturns) {
        negated.push_back(-r);
    }
    return generateSeriesFromReturns(ticker, negated, initialPrice);
}

// --- 3. Flat series (constant price) ---
inline PriceSeries generateFlatSeries(const std::string& ticker, size_t length, double price = 100.0) {
    std::vector<double> prices(length, price);
    std::vector<std::string> dates(length, "");
    return PriceSeries(ticker, dates, prices);
}

// --- 4. Noisy constant series (small +/- variation) ---
inline PriceSeries generateNoisySeries(const std::string& ticker, size_t length, double base = 100.0, double noise = 0.01) {
    std::vector<double> prices;
    double p = base;
    for (size_t i = 0; i < length; ++i) {
        double variation = 1.0 + ((i % 2 == 0) ? noise : -noise);
        p *= variation;
        prices.push_back(p);
    }
    std::vector<std::string> dates(length, "");
    return PriceSeries(ticker, dates, prices);
}

// --- 5. Random walk generator (Geometric Brownian Motion) ---
inline PriceSeries generateRandomWalkSeries(const std::string& ticker,
                                            size_t length,
                                            double startPrice = 100.0,
                                            double mu = 0.0002,
                                            double sigma = 0.01,
                                            unsigned int seed = 42) {
    std::default_random_engine gen(seed);
    std::normal_distribution<double> dist(mu, sigma);

    std::vector<double> prices = {startPrice};
    for (size_t i = 1; i < length; ++i) {
        double r = dist(gen);
        prices.push_back(prices.back() * std::exp(r));  // GBM step
    }
    std::vector<std::string> dates(length, "");
    return PriceSeries(ticker, dates, prices);
}

// --- 6. Simple equal-weight vector for N assets ---
inline std::vector<double> generateEqualWeights(size_t n) {
    if (n == 0) return {};
    return std::vector<double>(n, 1.0 / n);
}

// --- 7. Custom weights with normalization ---
inline std::vector<double> normalizeWeights(const std::vector<double>& rawWeights) {
    double sum = std::accumulate(rawWeights.begin(), rawWeights.end(), 0.0);
    std::vector<double> normed;
    for (double w : rawWeights)
        normed.push_back(w / sum);
    return normed;
}

// --- 8. Utility to compute cumulative return series from daily returns ---
inline std::vector<double> computeCumulativeReturns(const std::vector<double>& returns) {
    std::vector<double> cum;
    double val = 1.0;
    for (double r : returns) {
        val *= (1.0 + r);
        cum.push_back(val);
    }
    return cum;
}
