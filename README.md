# PGOF

Design problem for Parking Garage of the Future

You are a world renowned freelance software developer, just finished a crazy wild gig. You are relaxing in your favorite coffee shop, the Dangling Pointer, enjoying the grandeur of the best cappucino ever to grace the desperate hands of a barrista awaiting casting call. 

In walks Felon Tusk, the notorious giga entrepeneur. Felon scans the shop, immediately recognizes you by your weathered beret, walks over and takes a chair at your table. "I need your services, big money. I have this idea for an autonomous parking gargage platform called PGOF, Parking garage of the Future." Felon pulls out a wrinkled napkin full of scribbles. "Here are the specs. When can we go live?" You look at the specs, think for a moment. Why not, it's only Tuesday after all. You reply, "Tomorrow this time."  Felon nods curtly and scurries out of the coffee shop; while you pull out your lap top and get to work ...

## PGOF Software Requirments

![PGOF scene with queue, parking lots, and leaving cars](PGOF.drawio.svg)

PGOF is an evolving autonomous parking system for autonomous vehicles. PGOF must perform each of the following tasks: 

1. Each time PGOF starts it must check for the number of lots because the hardware is constantly replicating more parking spaces based on demand forecasts.
2. Autonomous vehicles join the waiting line in order of arrival.
3. Vehicles come in sizes of `{1, 2, 3}`.
4. Parking spaces come in sizes of `{1, 2, 3}`.
5. Each car arrives with a requested wait time `{1, 2, 3, 4, 5, .., 10}`.
6. The parking lot comprises N parking spaces with size `{1, 2, 3}`.
7. The following parking states are allowed:
   - space size = `{1, 2, 3}` holds no cars `{0}`
   - space size = `{1}` holds car = `{1}`
   - space size = `{2}` holds car = `{1, 2}`
   - space size = `{3}` holds car = `{1, 2, 3}` (e.g. a lot of size 3 can hold one car of size 1, 2, or 3)
8. PGOF parks each car in order of arrival until there are no empty spaces. After that PGOF can select the first eligible car to park.  PGOF returns to the original logic once spaces are available.
9. PGOF unparks cars once the wait time expires.
10. The goal is to maximize the fee for cars parked over a given time period `>> 10` time units. The fee structure is `fee = car_size * parking_time` (e.g. parking a car of size 3 for 2 time units costs $6)
11. Maintain a running sum of fees collected each time a car is parked.
12. Maintain a runing count of the total number of cars parked
13. Design and implement PGOF in C++.

## Key attributes of a successful solution are:

1. Ensuring top level use cases are well defined.
2. Definition of a system architecture that scales and satisfies best design practices and key quality attributes.
3. A data model that supports the problem.
4. Good use of C++ language features to implement PGOF.
5. A suite of tests that prove PGOF is defect free.

## Recommended Project structure 
```
project-root/
├── CMakeLists.txt
├── cmake/                 # CMake helper modules (Find*.cmake, presets, etc.)
├── include/               # Public headers (exported API)
│   └── project/           # Namespaced headers
├── src/                   # Core source for main project library
│   ├── CMakeLists.txt
│   └── *.cpp
├── libs/                  # Internal libraries (each self-contained)
│   ├── libA/
│   │   ├── include/libA/
│   │   ├── src/
│   │   └── CMakeLists.txt
│   └── libB/
│       ├── include/libB/
│       ├── src/
│       └── CMakeLists.txt
├── apps/                  # Applications / executables
│   ├── app1/
│   │   ├── src/
│   │   └── CMakeLists.txt
│   └── app2/
│       ├── src/
│       └── CMakeLists.txt
├── tests/                 # Unit tests (CTest, GoogleTest, etc.)
│   ├── CMakeLists.txt
│   └── test_*.cpp
├── docs/                  # Documentation (Doxygen, Sphinx, Markdown)
│   └── CMakeLists.txt
├── resources/             # Runtime assets (images, configs, shaders, etc.)
└──
```
## Garage Capacity Library

A new C++ library is available to compute the garage capacity split by space size.

Input:
- `N` = number of spaces in the garage
- valid range: `10 <= N <= 1000000`

Output:
- `N1` = number of size-1 spaces
- `N2` = number of size-2 spaces
- `N3` = number of size-3 spaces

API:
- Header: `libs/garage_capacity/include/pgof/garage_capacity.hpp`
- Function: `pgof::garage::calculate_garage_capacity(int total_spaces)`
- Return type: `pgof::garage::garage_capacity { n1, n2, n3 }`

Behavior:
- The implementation splits `N` as evenly as possible across `N1`, `N2`, and `N3`.
- If `N` is outside the valid range, the function throws `std::out_of_range`.

