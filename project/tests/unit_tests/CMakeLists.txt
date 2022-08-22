include_directories(../../src/transpiler)

include_directories(.)

add_executable(unit_tests
        # skiplist tests
        util_test.cpp
        #
        # skiplist sources
        ../../src/transpiler/util.h
        ../../src/transpiler/util.cpp
        #
        # gtest sources
        gtest/gtest-all.cc
        gtest/gtest_main.cc
        )

# add pthread for unix systems
if (UNIX)
    target_link_libraries(unit_tests pthread)
endif ()

# for coverage
# TARGET_COMPILE_OPTIONS(unit_tests PUBLIC --coverage)
# target_link_libraries(unit_tests gcov)