cmake_minimum_required(VERSION 3.18)

project(c2eo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type is Release" FORCE)
#set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(src/transpiler)

#todo: if u_test or rultor
add_subdirectory(tests/unit_tests)
