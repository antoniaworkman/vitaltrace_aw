cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_heart_rate)

target_sources(app PRIVATE
    src/test_heart_rate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/heart_rate_pb.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)