cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_battery)

target_sources(app PRIVATE
    src/test_battery.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/battery.c
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)