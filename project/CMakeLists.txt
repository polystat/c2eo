cmake_minimum_required(VERSION 3.18)

project(c2eo LANGUAGES CXX)

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(src/transpiler)
add_subdirectory(tests/unit_tests)
