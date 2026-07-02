cmake_minimum_required(VERSION 3.16)
project(canbus_decoder CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -O2)

include_directories(include)

add_library(canbus_core
    src/can_frame.cpp
    src/signal_table.cpp
    src/decoder.cpp
    src/simulator.cpp
)

add_executable(canbus_decoder src/main.cpp)
target_link_libraries(canbus_decoder canbus_core)

add_executable(bench_decoder bench/bench_decoder.cpp)
target_link_libraries(bench_decoder canbus_core)

enable_testing()
add_executable(unit_tests tests/test_decoder.cpp)
target_link_libraries(unit_tests canbus_core)
add_test(NAME unit_tests COMMAND unit_tests)

if(UNIX AND NOT APPLE)
    add_executable(canbus_decoder_live src/main_live.cpp src/socketcan_reader.cpp)
    target_link_libraries(canbus_decoder_live canbus_core)
endif()
