include_directories(../../src/transpiler)

include_directories(.)
enable_testing()
add_subdirectory(googletest)
add_executable(unit_tests
        #
        util_test.cpp
        #
        ../../src/transpiler/util.h
        ../../src/transpiler/util.cpp
        )

# add pthread for unix systems
if (UNIX)
    target_link_libraries(unit_tests pthread)
endif ()
target_link_libraries(unit_tests gtest_main gmock_main)

# for coverage
# TARGET_COMPILE_OPTIONS(unit_tests PUBLIC --coverage)
# target_link_libraries(unit_tests gcov)