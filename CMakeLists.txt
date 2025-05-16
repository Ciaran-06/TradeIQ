cmake_minimum_required(VERSION 3.16)
project(TradeIQ)

set(CMAKE_CXX_STANDARD 20)
enable_testing()
include(CTest)

# ========================================================
# Include FetchContent early
# ========================================================
include(FetchContent)

# ========================================================
# Download GoogleTest
# ========================================================
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ========================================================
# Fetch CPR (C++ Requests)
# ========================================================
FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.10.4
)

FetchContent_MakeAvailable(cpr)

# ========================================================
# Include directories
# ========================================================
include_directories(
  src
  src/api
  src/cli
  src/core
  src/stats
  src/utils
  src/external
  tests/include
)

# ========================================================
# Source files
# ========================================================
file(GLOB_RECURSE API_SRC src/api/*.cpp)
file(GLOB_RECURSE CORE_SRC src/core/*.cpp)
file(GLOB_RECURSE STATS_SRC src/stats/*.cpp)
file(GLOB_RECURSE CLI_SRC src/cli/*.cpp)
file(GLOB_RECURSE UTILS_SRC src/utils/*.cpp)
file(GLOB_RECURSE MAIN_SRC src/main.cpp)

# ========================================================
# Libraries
# ========================================================
add_library(api STATIC ${API_SRC})
target_link_libraries(api PUBLIC cpr::cpr)

add_library(core STATIC ${CORE_SRC})
add_library(stats STATIC ${STATS_SRC})
add_library(cli STATIC ${CLI_SRC})
add_library(math_utils STATIC ${UTILS_SRC})

# ========================================================
# Main Executable
# ========================================================
add_executable(main_exec ${MAIN_SRC})
target_link_libraries(main_exec
  PRIVATE api core stats cli math_utils
)
set_target_properties(main_exec PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ========================================================
# Tests
# ========================================================
file(GLOB_RECURSE TEST_SRC tests/*.cpp)

add_executable(tests ${TEST_SRC})

target_link_libraries(tests
  PRIVATE
    api
    core
    stats
    cli
    math_utils
    gtest
    gtest_main
)

# Put the test binary in the bin/ directory
set_target_properties(tests PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

add_test(NAME AllTests COMMAND tests)