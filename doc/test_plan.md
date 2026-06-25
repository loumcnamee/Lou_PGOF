# PGOF Test Plan

## 1. Overview

This document describes the test plan for the PGOF (Parking Garage of the Future) library. The test suite validates all functional requirements from the use cases and provides unit test coverage for all public APIs.

**Test Framework:** Native C++ with custom test harness  
**Test Location:** `tests/`  
**Build System:** CMake with CTest integration

---

## 2. Test Scope

### 2.1 In Scope

- All use cases defined in `doc/requirements.md` (UC-001 through UC-008)
- All public methods of PGOF library classes
- Boundary conditions and error handling
- EARS requirements validation

### 2.2 Out of Scope

- Performance/stress testing
- Integration with external hardware
- GUI/user interface testing

---

## 3. Test Executables

| Executable | Description | Source File |
|------------|-------------|-------------|
| `pgof_functional_tests` | Use case validation tests | `tests/pgof_functional_tests.cpp` |
| `pgof_unit_tests` | Class-level unit tests | `tests/pgof_unit_tests.cpp` |
| `garage_capacity_tests` | Capacity calculation tests | `tests/garage_capacity_tests.cpp` |

---

## 4. Functional Test Plan

Functional tests validate the system behavior as specified in the requirements document.

### 4.1 UC-001: System Initialization

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC001-01 | `test_UC001_system_initialization_valid_range` | Initialize garage with N=100 | Garage created with 100 spaces, balanced distribution, fees=0, cars_parked=0 |
| UC001-02 | `test_UC001_system_initialization_minimum` | Initialize garage with N=10 (minimum) | Garage created with at least one space of each size |
| UC001-03 | `test_UC001_system_initialization_maximum` | Initialize garage with N=1,000,000 (maximum) | Garage created with 1,000,000 spaces |
| UC001-04 | `test_UC001_system_initialization_invalid_low` | Initialize garage with N=9 | `std::out_of_range` exception thrown |
| UC001-05 | `test_UC001_system_initialization_invalid_high` | Initialize garage with N=1,000,001 | `std::out_of_range` exception thrown |
| UC001-06 | `test_UC001_reinitialize` | Re-initialize existing garage with new capacity | Total spaces updated to new value |

### 4.2 UC-002: Vehicle Arrival and Queue Entry

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC002-01 | `test_UC002_vehicle_arrival_adds_to_queue` | Add vehicle to garage | Queue size increases by 1 |
| UC002-02 | `test_UC002_fifo_queue_ordering` | Add 3 vehicles sequentially | All 3 vehicles in queue in arrival order |
| UC002-03 | `test_UC002_accept_all_valid_sizes` | Add vehicles with sizes 1, 2, 3 | All sizes accepted, queue size = 3 |
| UC002-04 | `test_UC002_accept_all_valid_wait_times` | Add vehicles with wait times 1-10 | All wait times accepted |

### 4.3 UC-003: Park Vehicle (Normal Mode - FIFO)

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC003-01 | `test_UC003_park_vehicle_fifo_order` | Park 2 vehicles in sequence | Vehicles parked in arrival order |
| UC003-02 | `test_UC003_parking_compatibility` | Park vehicles of each size | Each vehicle parked in compatible space |
| UC003-03 | `test_UC003_increments_parked_count` | Park one vehicle | Cars parked count = 1 |
| UC003-04 | `test_UC003_garage_mode_is_fifo_initially` | Check initial mode | Mode = FIFO |

### 4.4 UC-004: Park Vehicle (Selective Mode)

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC004-01 | `test_UC004_selective_mode_when_first_blocked` | Fill size-3 spaces, add large then small vehicle | Small vehicle parked, large remains in queue |
| UC004-02 | `test_UC004_returns_to_fifo_when_space_available` | Check mode after initialization | Mode starts as FIFO |

### 4.5 UC-005: Unpark Vehicle

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC005-01 | `test_UC005_unpark_expired_vehicle` | Park vehicle with wait_time=5, unpark at time 5 | 1 ticket returned |
| UC005-02 | `test_UC005_space_becomes_available_after_unpark` | Unpark vehicle | Occupied count decreases by 1 |
| UC005-03 | `test_UC005_no_unpark_before_expiry` | Try to unpark before wait time expires | No tickets returned |

### 4.6 UC-006: Calculate and Track Fees

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC006-01 | `test_UC006_fee_calculation_formula` | Park size-2 vehicle for 5 time units | Fee = 2 × 5 = 10 |
| UC006-02 | `test_UC006_running_total` | Unpark two vehicles sequentially | Running total accumulates |
| UC006-03 | `test_UC006_different_vehicle_sizes_fees` | Unpark vehicles of sizes 1, 2, 3 | Total fee = 4 + 8 + 12 = 24 |

### 4.7 UC-007: Track Parking Statistics

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC007-01 | `test_UC007_running_car_count` | Park multiple vehicles | Car count increments correctly |
| UC007-02 | `test_UC007_car_count_persists_after_unpark` | Unpark vehicle | Historical car count unchanged |
| UC007-03 | `test_UC007_fee_total_tracking` | Park and unpark large vehicle | Fees tracked correctly |

### 4.8 UC-008: Query Garage Status

| Test ID | Test Name | Description | Expected Result |
|---------|-----------|-------------|-----------------|
| UC008-01 | `test_UC008_query_total_spaces` | Query garage with 150 spaces | Status reports 150 total |
| UC008-02 | `test_UC008_query_space_distribution` | Query space distribution | n1 + n2 + n3 = total |
| UC008-03 | `test_UC008_query_occupied_spaces` | Park 2 vehicles, query status | Occupied = 2 |
| UC008-04 | `test_UC008_query_queue_length` | Add 2 vehicles, query status | Queue length = 2 |
| UC008-05 | `test_UC008_query_fees_and_car_count` | Park/unpark, query status | Correct fees and car count |
| UC008-06 | `test_UC008_available_spaces_calculation` | Park 1 vehicle in 100-space garage | Available = 99 |

---

## 5. Unit Test Plan

Unit tests validate individual class methods and edge cases.

### 5.1 Vehicle Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| VEH-01 | Constructor | `test_vehicle_constructor` | All attributes set correctly |
| VEH-02 | `getId()` | `test_vehicle_getId` | Returns correct ID |
| VEH-03 | `getSize()` | `test_vehicle_getSize` | Returns SMALL/MEDIUM/LARGE correctly |
| VEH-04 | `getRequestedWaitTime()` | `test_vehicle_getRequestedWaitTime` | Returns correct wait time |
| VEH-05 | `getArrivalTime()` | `test_vehicle_getArrivalTime` | Returns correct arrival time |
| VEH-06 | `canFitIn()` | `test_vehicle_canFitIn_small_vehicle` | Small fits in all spaces |
| VEH-07 | `canFitIn()` | `test_vehicle_canFitIn_medium_vehicle` | Medium fits in medium/large only |
| VEH-08 | `canFitIn()` | `test_vehicle_canFitIn_large_vehicle` | Large fits in large only |
| VEH-09 | `operator==` | `test_vehicle_equality_operator` | Same attributes → equal |
| VEH-10 | `operator!=` | `test_vehicle_inequality_operator` | Different IDs → not equal |

### 5.2 ParkingSpace Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| SPC-01 | Constructor | `test_parking_space_constructor` | ID/size set, unoccupied |
| SPC-02 | `getId()` | `test_parking_space_getId` | Returns correct ID |
| SPC-03 | `getSize()` | `test_parking_space_getSize` | Returns SMALL/MEDIUM/LARGE |
| SPC-04 | `isOccupied()` | `test_parking_space_isOccupied` | Returns correct state |
| SPC-05 | `getParkedVehicle()` | `test_parking_space_getParkedVehicle` | nullptr when empty, ptr when occupied |
| SPC-06 | `getParkingStartTime()` | `test_parking_space_getParkingStartTime` | Returns recorded start time |
| SPC-07 | `canAccommodate()` | `test_parking_space_canAccommodate_empty_space` | Size compatibility check |
| SPC-08 | `canAccommodate()` | `test_parking_space_canAccommodate_occupied_space` | Returns false when occupied |
| SPC-09 | `park()` | `test_parking_space_park` | Returns true, marks occupied |
| SPC-10 | `park()` | `test_parking_space_park_incompatible` | Returns false for large in small |
| SPC-11 | `park()` | `test_parking_space_park_already_occupied` | Returns false when occupied |
| SPC-12 | `unpark()` | `test_parking_space_unpark` | Returns vehicle, marks empty |
| SPC-13 | `unpark()` | `test_parking_space_unpark_empty` | Returns nullopt |

### 5.3 ParkingTicket Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| TKT-01 | Constructor | `test_parking_ticket_constructor` | All attributes set correctly |
| TKT-02 | `getTicketId()` | `test_parking_ticket_getTicketId` | Returns correct ID |
| TKT-03 | `getVehicleId()` | `test_parking_ticket_getVehicleId` | Returns correct vehicle ID |
| TKT-04 | `getSpaceId()` | `test_parking_ticket_getSpaceId` | Returns correct space ID |
| TKT-05 | `getStartTime()` | `test_parking_ticket_getStartTime` | Returns correct start time |
| TKT-06 | `getEndTime()` | `test_parking_ticket_getEndTime` | Returns correct end time |
| TKT-07 | `calculateFee()` | `test_parking_ticket_calculateFee` | fee = size × duration |
| TKT-08 | `getFee()` | `test_parking_ticket_getFee` | Returns pre-calculated fee |

### 5.4 WaitQueue Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| QUE-01 | `enqueue()` | `test_wait_queue_enqueue` | Adds vehicle to queue |
| QUE-02 | `dequeue()` | `test_wait_queue_dequeue` | Returns and removes front |
| QUE-03 | `dequeue()` | `test_wait_queue_dequeue_empty` | Throws runtime_error |
| QUE-04 | `front()` | `test_wait_queue_front` | Returns front without removing |
| QUE-05 | `front()` | `test_wait_queue_front_empty` | Throws runtime_error |
| QUE-06 | `findFirstEligible()` | `test_wait_queue_findFirstEligible` | Finds first fitting vehicle |
| QUE-07 | `findFirstEligible()` | `test_wait_queue_findFirstEligible_none_found` | Returns nullptr |
| QUE-08 | `findFirstEligible()` | `test_wait_queue_findFirstEligible_occupied_spaces` | Returns nullptr |
| QUE-09 | `remove()` | `test_wait_queue_remove` | Removes specific vehicle |
| QUE-10 | `remove()` | `test_wait_queue_remove_not_found` | Returns false |
| QUE-11 | `size()` | `test_wait_queue_size` | Returns correct count |
| QUE-12 | `isEmpty()` | `test_wait_queue_isEmpty` | Returns true/false correctly |
| QUE-13 | FIFO order | `test_wait_queue_fifo_order` | Maintains arrival order |
| QUE-14 | Iterators | `test_wait_queue_iterators` | begin()/end() work correctly |

### 5.5 Garage Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| GAR-01 | Constructor | `test_garage_constructor` | Initializes all fields |
| GAR-02 | Constructor | `test_garage_constructor_invalid` | Throws for invalid N |
| GAR-03 | `initialize()` | `test_garage_initialize` | Updates total spaces |
| GAR-04 | `initialize()` | `test_garage_initialize_invalid` | Throws for invalid N |
| GAR-05 | `addVehicle()` | `test_garage_addVehicle` | Adds to queue |
| GAR-06 | `parkNextVehicle()` | `test_garage_parkNextVehicle` | Parks and updates counts |
| GAR-07 | `parkNextVehicle()` | `test_garage_parkNextVehicle_empty_queue` | Returns false |
| GAR-08 | `unparkExpiredVehicles()` | `test_garage_unparkExpiredVehicles` | Returns tickets |
| GAR-09 | `unparkExpiredVehicles()` | `test_garage_unparkExpiredVehicles_multiple` | Handles multiple vehicles |
| GAR-10 | `getFeesCollected()` | `test_garage_getFeesCollected` | Returns running total |
| GAR-11 | `getCarsParked()` | `test_garage_getCarsParked` | Returns historical count |
| GAR-12 | `getStatus()` | `test_garage_getStatus` | Returns complete status |
| GAR-13 | `findAvailableSpace()` | `test_garage_findAvailableSpace` | Finds compatible space |
| GAR-14 | `setStrategy()` | `test_garage_setStrategy` | Accepts new strategy |
| GAR-15 | `getMode()` | `test_garage_getMode` | Returns FIFO initially |
| GAR-16 | `getQueueSize()` | `test_garage_getQueueSize` | Returns queue count |
| GAR-17 | `getTotalSpaces()` | `test_garage_getTotalSpaces` | Returns total |

### 5.6 FIFOStrategy Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| FIFO-01 | `selectVehicle()` | `test_fifo_strategy_selectVehicle` | Selects front vehicle |
| FIFO-02 | `selectVehicle()` | `test_fifo_strategy_selectVehicle_no_fit` | Returns nullptr |
| FIFO-03 | `selectVehicle()` | `test_fifo_strategy_selectVehicle_empty_queue` | Returns nullptr |
| FIFO-04 | `selectSpace()` | `test_fifo_strategy_selectSpace` | Finds compatible space |
| FIFO-05 | `selectSpace()` | `test_fifo_strategy_selectSpace_no_fit` | Returns nullptr |
| FIFO-06 | `selectSpace()` | `test_fifo_strategy_selectSpace_all_occupied` | Returns nullptr |

### 5.7 SelectiveStrategy Class

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| SEL-01 | `selectVehicle()` | `test_selective_strategy_selectVehicle` | Finds first eligible |
| SEL-02 | `selectVehicle()` | `test_selective_strategy_selectVehicle_first_fits` | Returns first if fits |
| SEL-03 | `selectVehicle()` | `test_selective_strategy_selectVehicle_none_fit` | Returns nullptr |
| SEL-04 | `selectSpace()` | `test_selective_strategy_selectSpace` | Finds compatible space |
| SEL-05 | `selectSpace()` | `test_selective_strategy_selectSpace_no_fit` | Returns nullptr |

### 5.8 GarageStatus Struct

| Test ID | Method | Test Name | Description |
|---------|--------|-----------|-------------|
| STS-01 | `getTotalOccupied()` | `test_garage_status_getTotalOccupied` | Sums occupied counts |
| STS-02 | `getTotalAvailable()` | `test_garage_status_getTotalAvailable` | total - occupied |
| STS-03 | Default values | `test_garage_status_default_values` | All fields initialize to 0 |

### 5.9 types.h Utilities

| Test ID | Function | Test Name | Description |
|---------|----------|-----------|-------------|
| TYP-01 | `canFit()` | `test_canFit_function` | All size combinations |
| TYP-02 | `Size` enum | `test_size_enum_values` | SMALL=1, MEDIUM=2, LARGE=3 |

---

## 6. Test Execution

### 6.1 Build Tests

```bash
cd build
cmake ..
cmake --build . --target pgof_functional_tests pgof_unit_tests
```

### 6.2 Run All Tests

```bash
cd build
ctest -V
```

### 6.3 Run Specific Test Suite

```bash
# Functional tests only
ctest -V -R pgof_functional

# Unit tests only
ctest -V -R pgof_unit
```

---

## 7. Test Coverage Summary

| Category | Test Count | Classes/Use Cases Covered |
|----------|------------|---------------------------|
| Functional Tests | 31 | 8 use cases (UC-001 to UC-008) |
| Unit Tests | 74 | 8 classes + utility functions |
| **Total** | **105** | Complete API coverage |

---

## 8. Pass/Fail Criteria

- **Pass:** All tests execute without failure, returning exit code 0
- **Fail:** Any test assertion fails, causing immediate test termination

---

## 9. Test Artifacts

| Artifact | Location |
|----------|----------|
| Functional Test Source | `tests/pgof_functional_tests.cpp` |
| Unit Test Source | `tests/pgof_unit_tests.cpp` |
| Test CMake Configuration | `tests/CMakeLists.txt` |
| Test Results | `build/Testing/Temporary/LastTest.log` |
| Requirements Document | `doc/requirements.md` |

---

## 10. Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-06-25 | PGOF Team | Initial test plan creation |
