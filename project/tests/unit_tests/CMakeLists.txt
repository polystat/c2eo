include_directories(
        .
        ../../src/transpiler
)

find_package(GTest REQUIRED)

add_executable(unit_tests
        #
        ./util_test.cpp
        #
        ../../src/transpiler/util.h
        ../../src/transpiler/util.cpp
        )

# add pthread for unix systems
if (UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(
            unit_tests
            ${GTEST_LIBRARIES}
            Threads::Threads
    )
endif ()

target_link_libraries(unit_tests gtest_main gmock_main)

enable_testing()
add_test(unit_tests "./unit_tests")

# for coverage
# TARGET_COMPILE_OPTIONS(unit_tests PUBLIC --coverage)
# target_link_libraries(unit_tests gcov)
