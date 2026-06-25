add_executable(garage_capacity_tests
    garage_capacity_tests.cpp
)

target_link_libraries(garage_capacity_tests
    PRIVATE
        pgof::garage_capacity
)

add_test(NAME garage_capacity_tests COMMAND garage_capacity_tests)

# PGOF Library Functional Tests
add_executable(pgof_functional_tests
    pgof_functional_tests.cpp
)

target_link_libraries(pgof_functional_tests
    PRIVATE
        pgof::pgof
)

add_test(NAME pgof_functional_tests COMMAND pgof_functional_tests)

# PGOF Library Unit Tests
add_executable(pgof_unit_tests
    pgof_unit_tests.cpp
)

target_link_libraries(pgof_unit_tests
    PRIVATE
        pgof::pgof
)

add_test(NAME pgof_unit_tests COMMAND pgof_unit_tests)