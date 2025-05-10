cmake_minimum_required(VERSION 3.14)
project(TradeIQ)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Dependencies via vcpkg ===
find_package(cpr REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Catch2 REQUIRED)

# === Main Executable ===
add_executable(main
    main.cpp
    src/core/price_series.cpp
    src/api/tiingo_client.cpp
)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api
    ${CMAKE_CURRENT_SOURCE_DIR}/external/dotenv-cpp/include
)

target_link_libraries(main PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
)

# === Test Executable ===
add_executable(tests
    tests/test_tiingo_client.cpp
    src/core/price_series.cpp
    src/api/tiingo_client.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api
    ${CMAKE_CURRENT_SOURCE_DIR}/external/dotenv-cpp/include
)

target_link_libraries(tests PRIVATE
    cpr::cpr
    nlohmann_json::nlohmann_json
    Catch2::Catch2WithMain
)

enable_testing()
add_test(NAME TradeIQTests COMMAND tests)
