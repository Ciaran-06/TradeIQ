#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "api/tiingo_client.hpp"
#include "core/price_series.hpp"

#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("TiingoClient input validation", "[validation]") {
    TiingoClient client;

    REQUIRE_THROWS_AS(client.fetchDailyPrices("", "2023-01-01", "2023-01-31"), std::invalid_argument);
    REQUIRE_THROWS_AS(client.fetchDailyPrices("AAPL", "2023-01-32", "2023-01-31"), std::invalid_argument);
    REQUIRE_THROWS_AS(client.fetchDailyPrices("AAPL", "2023-01-01", ""), std::invalid_argument);
}

TEST_CASE("TiingoClient throws if cache is missing in offline mode", "[cache][error]") {
    TiingoClient client;
    client.setOfflineMode(true);

    REQUIRE_THROWS_WITH(
        client.fetchDailyPrices("NONEXISTENT", "2000-01-01", "2000-01-02"),
        Catch::Matchers::ContainsSubstring("Offline mode enabled and no cache found")
    );
}

TEST_CASE("TiingoClient fetch and parse (cached)", "[cache]") {
    std::string fakeData = R"([
        {"date": "2023-01-01", "adjClose": 150.0},
        {"date": "2023-01-02", "adjClose": 152.0}
    ])";

    fs::create_directories(".cache");
    std::ofstream out(".cache/FAKE_2023-01-01_2023-01-02.json");
    out << fakeData;
    out.close();

    TiingoClient client;
    client.setOfflineMode(true);

    PriceSeries ps = client.fetchDailyPrices("FAKE", "2023-01-01", "2023-01-02");

    REQUIRE(ps.getTicker() == "FAKE");
    REQUIRE(ps.getPrices().size() == 2);
    REQUIRE(ps.getPrices()[0] == Catch::Approx(150.0));
    REQUIRE(ps.getPrices()[1] == Catch::Approx(152.0));
}

TEST_CASE("TiingoClient multi-ticker fetch (cached)", "[multi-cache]") {
    std::string data1 = R"([
        {"date": "2023-01-01", "adjClose": 100.0},
        {"date": "2023-01-02", "adjClose": 102.0}
    ])";

    std::string data2 = R"([
        {"date": "2023-01-01", "adjClose": 200.0},
        {"date": "2023-01-02", "adjClose": 202.0}
    ])";

    fs::create_directories(".cache");
    std::ofstream(".cache/AAA_2023-01-01_2023-01-02.json") << data1;
    std::ofstream(".cache/BBB_2023-01-01_2023-01-02.json") << data2;

    TiingoClient client;
    client.setOfflineMode(true);

    auto result = client.fetchMultipleDailyPrices({"AAA", "BBB"}, "2023-01-01", "2023-01-02");

    REQUIRE(result.size() == 2);
    REQUIRE(result.count("AAA") == 1);
    REQUIRE(result.count("BBB") == 1);

    REQUIRE(result.at("AAA").getPrices().at(0) == Catch::Approx(100.0));
    REQUIRE(result.at("BBB").getPrices().at(1) == Catch::Approx(202.0));
}

TEST_CASE("TiingoClient live fetch (dev only)", "[api][live][!mayfail]") {
    const char* key = std::getenv("API_KEY");
    if (!key) return;

    TiingoClient client(key, nullptr);
    client.setOfflineMode(false);

    PriceSeries ps = client.fetchDailyPrices("AAPL", "2023-01-03", "2023-01-05");

    REQUIRE(ps.getTicker() == "AAPL");
    REQUIRE(ps.getPrices().size() >= 2);
    REQUIRE(ps.getDates().size() == ps.getPrices().size());
}
