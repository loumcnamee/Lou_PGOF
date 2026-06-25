# GoogleTest-based tests - all tests in a single executable
include(GoogleTest)

add_executable(pgof_tests
    garage_capacity_tests.cpp
    pgof_functional_tests.cpp
    pgof_unit_tests.cpp
)

target_link_libraries(pgof_tests
    PRIVATE
        pgof::pgof
        pgof::garage_capacity
        GTest::gtest_main
)

gtest_discover_tests(pgof_tests)