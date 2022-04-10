# !!!!
# Это начальный файл проекта, который впоследствии должен быть
# переработан и настроен на отделные подпроекты!!!!

cmake_minimum_required(VERSION 3.18)

project(c2eo LANGUAGES CXX)

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$BUILD_DIR}/../lib")

add_subdirectory(src/transpiler)
#add_subdirectory(src/launcher)
#add_subdirectory(src/collector)
