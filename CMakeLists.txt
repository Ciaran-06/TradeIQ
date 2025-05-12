cmake_minimum_required(VERSION 3.14)
project(TradeIQ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find dependencies
find_package(cpr REQUIRED)
find_package(Catch2 3 REQUIRED)

# Include dirs
include_directories(src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/external)

# Core library
add_library(core
    src/core/price_series.cpp
    src/core/stats_engine.cpp
    src/core/strategy_engine.cpp
)

# API library
add_library(api
    src/api/tiingo_client.cpp
    src/api/default_http_client.cpp
)

target_link_libraries(api PUBLIC core cpr::cpr)

# CLI utils (e.g., matrix printer)
add_library(cli_utils
    src/cli/matrix_printer.cpp
)

target_link_libraries(cli_utils PUBLIC core)

# Main executable
add_executable(main
    src/main.cpp
)

target_link_libraries(main PRIVATE api core cli_utils cpr::cpr)

# Tests
enable_testing()

add_executable(tests
    tests/test_tiingo_client.cpp
    tests/test_stats_engine.cpp
)

target_link_libraries(tests
    PRIVATE
    api
    core
    Catch2::Catch2WithMain
)

include(CTest)
