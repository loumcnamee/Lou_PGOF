# PGOF Requirements Document

## 1. Overview

PGOF (Parking Garage of the Future) is an autonomous parking system designed to manage autonomous
 vehicles efficiently. The system maximizes parking fees by intelligently assigning vehicles to
 parking spaces based on size compatibility and arrival order.

---

## 2. Use Cases

### UC-001: System Initialization

**Actor:** PGOF System  
**Precondition:** System is starting up  
**Description:** Initialize the parking garage with the current number of available parking spaces.

**Main Flow:**

1. PGOF starts up
2. System queries hardware for the total number of parking spaces (N)
3. System validates N is within valid range (10 ≤ N ≤ 1,000,000)
4. System calculates space distribution using garage capacity library
5. System initializes parking space data structures with sizes {1, 2, 3}
6. System initializes fee counter to zero
7. System initializes parked car counter to zero
8. System is ready to accept vehicles

**Postcondition:** Garage is initialized with N1 size-1 spaces, N2 size-2 spaces, and N3 size-3 spaces  
**Exception:** If N is out of valid range, system throws `std::out_of_range`

**EARS Requirement:**

- WHEN PGOF starts, THE SYSTEM SHALL query the hardware for the current number of parking spaces.
- WHEN PGOF starts, THE SYSTEM SHALL calculate the distribution of space sizes {1, 2, 3}.

- Note: EARS stands for Easy Approach to Requirements Syntax — a standard format for writing clear,
 testable requirements

---

### UC-002: Vehicle Arrival and Queue Entry

**Actor:** Autonomous Vehicle  
**Precondition:** PGOF system is running  
**Description:** An autonomous vehicle arrives and joins the waiting queue.

**Main Flow:**

1. Autonomous vehicle arrives at PGOF entrance
2. Vehicle provides its size (1, 2, or 3)
3. Vehicle provides requested wait time (1-10 time units)
4. System adds vehicle to the end of the waiting queue
5. System confirms vehicle is queued

**Postcondition:** Vehicle is in the waiting queue in order of arrival  
**Alternative Flow:** If vehicle size or wait time is invalid, reject the vehicle

**EARS Requirement:**

- WHEN an autonomous vehicle arrives, THE SYSTEM SHALL add it to the waiting queue in order of arrival.
- THE SYSTEM SHALL accept vehicles with sizes {1, 2, 3}.
- THE SYSTEM SHALL accept requested wait times in the range {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}.

---

### UC-003: Park Vehicle (Normal Mode)

**Actor:** PGOF System  
**Precondition:** Vehicles are in the waiting queue AND parking spaces are available  
**Description:** Park vehicles from the queue when spaces are available.

**Main Flow:**

1. System checks if parking spaces are available
2. System retrieves the first vehicle from the queue (FIFO order)
3. System finds a compatible parking space:
   - Size-1 vehicle → Size-1, Size-2, or Size-3 space
   - Size-2 vehicle → Size-2 or Size-3 space
   - Size-3 vehicle → Size-3 space only
4. System assigns vehicle to the parking space
5. System records parking start time
6. System increments parked car counter
7. System removes vehicle from waiting queue

**Postcondition:** Vehicle is parked in a compatible space  
**Alternative Flow:** If no compatible space for first vehicle, switch to UC-004 (Selective Parking)

**EARS Requirement:**

- WHILE parking spaces are available, THE SYSTEM SHALL park vehicles in order of arrival (FIFO).
- WHEN parking a vehicle, THE SYSTEM SHALL assign it to a space where space_size ≥ vehicle_size.

---

### UC-004: Park Vehicle (Selective Mode)

**Actor:** PGOF System  
**Precondition:** Parking spaces are full for the first vehicle in queue  
**Description:** When the first vehicle cannot be parked, select the first eligible vehicle from the queue.

**Main Flow:**

1. System determines no space is available for the first vehicle
2. System scans queue for the first vehicle that fits an available space
3. System parks the eligible vehicle (if found)
4. System increments parked car counter
5. System removes parked vehicle from queue
6. System returns to normal mode when spaces become available for first vehicle

**Postcondition:** First eligible vehicle from queue is parked  
**Alternative Flow:** If no vehicle in queue can be parked, wait for spaces to free up

**EARS Requirement:**

- WHEN there are no empty spaces for the first vehicle in queue, THE SYSTEM SHALL select the first eligible vehicle from the queue to park.
- WHEN parking spaces become available for the first vehicle in queue, THE SYSTEM SHALL return to FIFO parking order.

---

### UC-005: Unpark Vehicle

**Actor:** PGOF System  
**Precondition:** A parked vehicle's wait time has expired  
**Description:** Remove a vehicle from its parking space when its wait time expires.

**Main Flow:**

1. System monitors parked vehicles' wait times
2. System detects a vehicle's wait time has expired
3. System calculates parking fee: `fee = vehicle_size × parking_time`
4. System adds fee to running total
5. System removes vehicle from parking space
6. System marks parking space as available
7. Vehicle exits the garage

**Postcondition:** Vehicle has exited, space is available, fee is collected  

**EARS Requirement:**

- WHEN a parked vehicle's wait time expires, THE SYSTEM SHALL unpark the vehicle.
- WHEN unparking a vehicle, THE SYSTEM SHALL calculate fee as `vehicle_size × parking_time`.
- WHEN unparking a vehicle, THE SYSTEM SHALL add the fee to the running total.

---

### UC-006: Calculate and Track Fees

**Actor:** PGOF System  
**Precondition:** Vehicle is being unparked  
**Description:** Calculate parking fee and maintain running totals.

**Main Flow:**

1. System calculates fee: `fee = vehicle_size × parking_time`
2. System adds fee to running sum
3. System logs fee transaction

**Postcondition:** Fee is added to running total  

**EARS Requirement:**

- THE SYSTEM SHALL maintain a running sum of fees collected each time a vehicle is parked.
- THE SYSTEM SHALL calculate fees using the formula: `fee = car_size × parking_time`.

---

### UC-007: Track Parking Statistics

**Actor:** PGOF System  
**Precondition:** PGOF system is operational  
**Description:** Maintain statistics about parking operations.

**Main Flow:**

1. System increments car count each time a vehicle is parked
2. System updates fee total each time a vehicle is unparked
3. System provides statistics on request

**Postcondition:** Statistics are current and accurate  

**EARS Requirement:**

- THE SYSTEM SHALL maintain a running count of the total number of cars parked.
- THE SYSTEM SHALL maintain a running sum of fees collected.

---

### UC-008: Query Garage Status

**Actor:** External System / Administrator  
**Precondition:** PGOF system is running  
**Description:** Query the current state of the parking garage.

**Main Flow:**

1. Actor requests garage status
2. System returns:
   - Total spaces (N) and distribution (N1, N2, N3)
   - Number of occupied spaces by size
   - Number of vehicles in queue
   - Total fees collected
   - Total cars parked

**Postcondition:** Status information is returned  

**EARS Requirement:**

- WHEN requested, THE SYSTEM SHALL provide current garage status including occupancy and statistics.

---

## 3. Domain Model

### 3.1 Entities

| Entity | Attributes | Description |
|--------|------------|-------------|
| **Vehicle** | id, size {1,2,3}, requested_wait_time {1..10}, arrival_time | Autonomous vehicle seeking parking |
| **ParkingSpace** | id, size {1,2,3}, occupied, parked_vehicle_id | Physical parking space |
| **Garage** | total_spaces, spaces[], queue[], fees_collected, cars_parked | The parking facility |
| **ParkingTicket** | vehicle_id, space_id, start_time, end_time, fee | Record of parking transaction |

### 3.2 Parking Compatibility Matrix

| Vehicle Size | Compatible Space Sizes |
|--------------|------------------------|
| 1 | 1, 2, 3 |
| 2 | 2, 3 |
| 3 | 3 |

---

## 4. Functional Requirements (EARS Notation)

### FR-001: Garage Initialization

- WHEN PGOF starts, THE SYSTEM SHALL check for the number of lots from hardware.
- THE SYSTEM SHALL support parking spaces with sizes {1, 2, 3}.
- IF the number of spaces N is outside range [10, 1000000], THEN THE SYSTEM SHALL throw `std::out_of_range`.

### FR-002: Vehicle Queue Management

- WHEN a vehicle arrives, THE SYSTEM SHALL add it to the waiting queue in FIFO order.
- THE SYSTEM SHALL accept vehicle sizes {1, 2, 3}.
- THE SYSTEM SHALL accept wait times {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}.

### FR-003: Parking Logic

- WHILE spaces are available, THE SYSTEM SHALL park vehicles in order of arrival.
- WHEN no space fits the first vehicle, THE SYSTEM SHALL park the first eligible vehicle from queue.
- WHEN spaces become available, THE SYSTEM SHALL return to FIFO order.
- THE SYSTEM SHALL only park a vehicle in a space where space_size ≥ vehicle_size.

### FR-004: Unparking Logic

- WHEN a vehicle's wait time expires, THE SYSTEM SHALL unpark the vehicle.
- WHEN unparking, THE SYSTEM SHALL free the parking space.

### FR-005: Fee Management

- WHEN a vehicle is parked, THE SYSTEM SHALL record the start time.
- WHEN a vehicle is unparked, THE SYSTEM SHALL calculate fee = vehicle_size × parking_time.
- THE SYSTEM SHALL maintain a running sum of all fees collected.

### FR-006: Statistics

- THE SYSTEM SHALL maintain a running count of total cars parked.
- THE SYSTEM SHALL maximize fees over time periods >> 10 time units.

---

## 5. Non-Functional Requirements

### NFR-001: Scalability

- THE SYSTEM SHALL support garages with 10 to 1,000,000 parking spaces.

### NFR-002: Implementation

- THE SYSTEM SHALL be implemented in C++.
- THE SYSTEM SHALL follow best design practices and quality attributes.

### NFR-003: Testability

- THE SYSTEM SHALL include a test suite proving defect-free operation.

### NFR-004: Extensibility

- THE SYSTEM SHALL be designed with an architecture that scales.

---

## 6. Constraints

1. Vehicle sizes are limited to {1, 2, 3}
2. Parking space sizes are limited to {1, 2, 3}
3. Wait times are limited to {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} time units
4. Total spaces must be in range [10, 1,000,000]
5. One parking space holds at most one vehicle

---

## 7. Acceptance Criteria

| ID | Criterion |
|----|-----------|
| AC-001 | System initializes with correct space distribution |
| AC-002 | Vehicles queue in arrival order |
| AC-003 | FIFO parking when spaces available |
| AC-004 | Selective parking when FIFO not possible |
| AC-005 | Automatic unparking when wait time expires |
| AC-006 | Correct fee calculation: size × time |
| AC-007 | Accurate running fee total |
| AC-008 | Accurate parked car count |
| AC-009 | Space compatibility rules enforced |
| AC-010 | System handles 10 to 1,000,000 spaces |
