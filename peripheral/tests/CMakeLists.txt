cmake_minimum_required(VERSION 3.20.0)
project(vitaltrace_tests C)

set(CMAKE_C_STANDARD 11)

# Unity source
set(UNITY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/unity/src/unity.c)

# Paths
set(SRC_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(NANOPB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/nanopb)

# -----------------------------------------------------------------------
# Stubs — minimal implementations of Zephyr/BT APIs for host compilation
# -----------------------------------------------------------------------
add_library(zephyr_stubs STATIC
    stubs/bt_bas_stub.c
    stubs/bt_gatt_stub.c
    stubs/kernel_stub.c
)
target_include_directories(zephyr_stubs PUBLIC
    stubs/
    ${INCLUDE_DIR}
)

# -----------------------------------------------------------------------
# Battery test executable
# -----------------------------------------------------------------------
add_executable(test_battery
    ${UNITY_SRC}
    battery/test_battery.c
    ${SRC_DIR}/battery.c
)
target_include_directories(test_battery PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/src
    ${INCLUDE_DIR}
    stubs/
)
target_link_libraries(test_battery PRIVATE zephyr_stubs)
add_test(NAME battery COMMAND test_battery)

# -----------------------------------------------------------------------
# Heart rate / protobuf test executable
# -----------------------------------------------------------------------
add_executable(test_heart_rate
    ${UNITY_SRC}
    heart_rate/test_heart_rate.c
    ${SRC_DIR}/heart_rate_pb.c
    ${NANOPB_DIR}/pb_encode.c
    ${NANOPB_DIR}/pb_decode.c
    ${NANOPB_DIR}/pb_common.c
)
target_include_directories(test_heart_rate PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unity/src
    ${INCLUDE_DIR}
    stubs/
    ${NANOPB_DIR} 
)
add_test(NAME heart_rate COMMAND test_heart_rate)

# Enable ctest
enable_testing()
