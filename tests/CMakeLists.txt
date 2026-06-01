add_executable(garage_capacity_tests
    garage_capacity_tests.cpp
)

target_link_libraries(garage_capacity_tests
    PRIVATE
        pgof::garage_capacity
)

add_test(NAME garage_capacity_tests COMMAND garage_capacity_tests)