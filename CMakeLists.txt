cmake_minimum_required(VERSION 3.15)
project(TradeIQ VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Core engine
add_subdirectory(src)

# Optional: Python interface
option(ENABLE_PYTHON "Enable Python bindings" OFF)
if(ENABLE_PYTHON)
  add_subdirectory(python)
endif()

# Tests
enable_testing()
add_subdirectory(tests)
