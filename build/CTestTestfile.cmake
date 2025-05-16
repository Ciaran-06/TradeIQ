# CMake generated Testfile for 
# Source directory: /Users/ciarangaffney/TradeIQ
# Build directory: /Users/ciarangaffney/TradeIQ/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(AllTests "/Users/ciarangaffney/TradeIQ/build/bin/tests")
set_tests_properties(AllTests PROPERTIES  _BACKTRACE_TRIPLES "/Users/ciarangaffney/TradeIQ/CMakeLists.txt;106;add_test;/Users/ciarangaffney/TradeIQ/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
subdirs("_deps/cpr-build")
