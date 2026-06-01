
# List all source and header files for dependency tracking
set(GARAGE_CAPACITY_SOURCES
    src/garage_capacity.cpp
)
set(GARAGE_CAPACITY_HEADERS
    include/garage_capacity.h
)

add_library(garage_capacity
    ${GARAGE_CAPACITY_SOURCES}
    ${GARAGE_CAPACITY_HEADERS}
)

add_library(pgof::garage_capacity ALIAS garage_capacity)

target_include_directories(garage_capacity
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)