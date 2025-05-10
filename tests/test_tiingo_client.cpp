#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "tiingo_client.hpp"

#include <fstream>
#include <string>

namespace fs = std::filesystem;


TEST_CASE("TiingoClient input validation", "[validation]") {
    TiingoClient client;

    REQUIRE_THROWS_AS(client.fetchDailyPrices("", "2023-01-01", "2023-01-31"), std::invalid_argument);
    REQUIRE_THROWS_AS(client.fetchDailyPrices("AAPL", "2023-01-32", "2023-01-31"), std::invalid_argument);
    REQUIRE_THROWS_AS(client.fetchDailyPrices("AAPL", "2023-01-01", ""), std::invalid_argument);
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
    REQUIRE(result.count("AAA") == 1);
    REQUIRE(result.count("BBB") == 1);
    REQUIRE(result.at("AAA").getPrices().at(0) == Catch::Approx(100.0));
    REQUIRE(result.at("BBB").getPrices().at(1) == Catch::Approx(202.0));
}
