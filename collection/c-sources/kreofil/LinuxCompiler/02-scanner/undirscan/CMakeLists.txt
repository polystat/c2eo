cmake_minimum_required(VERSION 3.0) # Проверка версии CMake.
                                    # Если версия установленой программы
                                    # старее указаной, произайдёт аварийный выход.

project(dpl)               # Название проекта

set(BUILD_DIR "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BUILD_DIR}/../bin")

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type is Debug" FORCE)


# Список исходных файлов
set(SOURCE_FILES
    # Headers
    ../er_d.h
    ../scaner_d.h
    ../main_c.h
    ../main_d.h
    ../main_e.h
    ../main_t.h
    
    # Sources
    ../main.c
    ../er.c
    ../scaner.c
    ../testout.c    
)

add_executable(undir_scan ${SOURCE_FILES})  # Создает исполняемый файл с именем dir_scan
