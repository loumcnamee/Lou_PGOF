# PGOF Core Library - Parking Garage of the Future
# Contains core domain classes for the parking system

# List all source files
set(PGOF_SOURCES
    src/vehicle.cpp
    src/parking_space.cpp
    src/parking_ticket.cpp
    src/wait_queue.cpp
    src/fifo_strategy.cpp
    src/selective_strategy.cpp
    src/garage.cpp
)

# List all header files
set(PGOF_HEADERS
    include/pgof/types.h
    include/pgof/vehicle.h
    include/pgof/parking_space.h
    include/pgof/parking_ticket.h
    include/pgof/garage_status.h
    include/pgof/wait_queue.h
    include/pgof/i_parking_strategy.h
    include/pgof/fifo_strategy.h
    include/pgof/selective_strategy.h
    include/pgof/garage.h
)

add_library(pgof
    ${PGOF_SOURCES}
    ${PGOF_HEADERS}
)

add_library(pgof::pgof ALIAS pgof)

target_include_directories(pgof
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link to garage_capacity library for initialization
target_link_libraries(pgof
    PUBLIC
        pgof::garage_capacity
)

# Require C++17 for std::optional
target_compile_features(pgof PUBLIC cxx_std_17)
