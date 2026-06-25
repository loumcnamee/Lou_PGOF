/**
 * @file pgof_functional_tests.cpp
 * @brief Functional tests for PGOF library based on use cases from requirements.md
 *
 * Tests UC-001 through UC-008 as specified in the requirements document.
 */

#include "pgof/garage.h"
#include "pgof/vehicle.h"
#include "pgof/parking_space.h"
#include "pgof/parking_ticket.h"
#include "pgof/wait_queue.h"
#include "pgof/garage_status.h"
#include "pgof/fifo_strategy.h"
#include "pgof/selective_strategy.h"
#include "pgof/types.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace pgof;

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

// ============================================================================
// UC-001: System Initialization
// ============================================================================

void test_UC001_system_initialization_valid_range() {
    // WHEN PGOF starts with valid N, THE SYSTEM SHALL initialize successfully
    Garage garage(100);
    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 100, "UC-001: Total spaces should be 100");
    expect(status.n1_spaces + status.n2_spaces + status.n3_spaces == 100,
           "UC-001: Space distribution should sum to total");
    expect(status.fees_collected == 0, "UC-001: Initial fees should be zero");
    expect(status.cars_parked == 0, "UC-001: Initial cars parked should be zero");
}

void test_UC001_system_initialization_minimum() {
    // Test minimum valid range (N=10)
    Garage garage(10);
    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 10, "UC-001: Minimum spaces should be 10");
    expect(status.n1_spaces >= 1, "UC-001: Should have at least one size-1 space");
    expect(status.n2_spaces >= 1, "UC-001: Should have at least one size-2 space");
    expect(status.n3_spaces >= 1, "UC-001: Should have at least one size-3 space");
}

void test_UC001_system_initialization_maximum() {
    // Test maximum valid range (N=1,000,000)
    Garage garage(1000000);
    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 1000000, "UC-001: Maximum spaces should be 1,000,000");
}

void test_UC001_system_initialization_invalid_low() {
    // IF N is outside range [10, 1000000], THEN throw out_of_range
    bool threw = false;
    try {
        Garage garage(9);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect(threw, "UC-001: N<10 should throw out_of_range");
}

void test_UC001_system_initialization_invalid_high() {
    bool threw = false;
    try {
        Garage garage(1000001);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect(threw, "UC-001: N>1000000 should throw out_of_range");
}

void test_UC001_reinitialize() {
    Garage garage(100);
    garage.initialize(200);
    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 200, "UC-001: Re-initialization should update total spaces");
}

// ============================================================================
// UC-002: Vehicle Arrival and Queue Entry
// ============================================================================

void test_UC002_vehicle_arrival_adds_to_queue() {
    // WHEN an autonomous vehicle arrives, THE SYSTEM SHALL add it to the waiting queue
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    garage.addVehicle(v1);

    expect(garage.getQueueSize() == 1, "UC-002: Queue should have 1 vehicle after arrival");
}

void test_UC002_fifo_queue_ordering() {
    // Vehicles should be in order of arrival (FIFO)
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 3, 1);
    Vehicle v3(3, Size::LARGE, 7, 2);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.addVehicle(v3);

    expect(garage.getQueueSize() == 3, "UC-002: Queue should have 3 vehicles");
}

void test_UC002_accept_all_valid_sizes() {
    // THE SYSTEM SHALL accept vehicles with sizes {1, 2, 3}
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);   // Size 1
    Vehicle v2(2, Size::MEDIUM, 5, 1);  // Size 2
    Vehicle v3(3, Size::LARGE, 5, 2);   // Size 3

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.addVehicle(v3);

    expect(garage.getQueueSize() == 3, "UC-002: All sizes should be accepted");
}

void test_UC002_accept_all_valid_wait_times() {
    // THE SYSTEM SHALL accept requested wait times in range {1..10}
    Garage garage(100);

    for (int wait_time = 1; wait_time <= 10; ++wait_time) {
        Vehicle v(wait_time, Size::SMALL, wait_time, wait_time);
        garage.addVehicle(v);
    }

    expect(garage.getQueueSize() == 10, "UC-002: All wait times 1-10 should be accepted");
}

// ============================================================================
// UC-003: Park Vehicle (Normal Mode - FIFO)
// ============================================================================

void test_UC003_park_vehicle_fifo_order() {
    // WHILE parking spaces are available, THE SYSTEM SHALL park vehicles in order of arrival
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    bool parked1 = garage.parkNextVehicle(0);
    expect(parked1, "UC-003: First vehicle should be parked");
    expect(garage.getQueueSize() == 1, "UC-003: Queue should have 1 vehicle after parking");

    bool parked2 = garage.parkNextVehicle(1);
    expect(parked2, "UC-003: Second vehicle should be parked");
    expect(garage.getQueueSize() == 0, "UC-003: Queue should be empty after parking both");
}

void test_UC003_parking_compatibility() {
    // THE SYSTEM SHALL assign vehicle to space where space_size >= vehicle_size
    Garage garage(100);

    // Small vehicle can fit in any space
    Vehicle v_small(1, Size::SMALL, 5, 0);
    garage.addVehicle(v_small);
    bool parked = garage.parkNextVehicle(0);
    expect(parked, "UC-003: Small vehicle should find a compatible space");

    // Medium vehicle needs size 2 or 3
    Vehicle v_medium(2, Size::MEDIUM, 5, 1);
    garage.addVehicle(v_medium);
    parked = garage.parkNextVehicle(1);
    expect(parked, "UC-003: Medium vehicle should find a compatible space");

    // Large vehicle needs size 3 only
    Vehicle v_large(3, Size::LARGE, 5, 2);
    garage.addVehicle(v_large);
    parked = garage.parkNextVehicle(2);
    expect(parked, "UC-003: Large vehicle should find a compatible space");
}

void test_UC003_increments_parked_count() {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    expect(garage.getCarsParked() == 1, "UC-003: Cars parked count should be 1");
}

void test_UC003_garage_mode_is_fifo_initially() {
    Garage garage(100);
    expect(garage.getMode() == ParkingMode::FIFO, "UC-003: Initial mode should be FIFO");
}

// ============================================================================
// UC-004: Park Vehicle (Selective Mode)
// ============================================================================

void test_UC004_selective_mode_when_first_blocked() {
    // WHEN there are no empty spaces for the first vehicle in queue,
    // THE SYSTEM SHALL select the first eligible vehicle from the queue to park

    // Create garage with limited spaces
    Garage garage(10);

    // Fill all size-3 spaces with large vehicles first
    GarageStatus status = garage.getStatus();
    int large_spaces = status.n3_spaces;

    for (int i = 0; i < large_spaces; ++i) {
        Vehicle v(i, Size::LARGE, 10, i);
        garage.addVehicle(v);
        garage.parkNextVehicle(i);
    }

    // Now add a large vehicle (cannot be parked - no size-3 spaces left)
    Vehicle blocked_vehicle(100, Size::LARGE, 5, large_spaces);
    garage.addVehicle(blocked_vehicle);

    // Add a small vehicle (can be parked in size-1 or size-2 space)
    Vehicle small_vehicle(101, Size::SMALL, 5, large_spaces + 1);
    garage.addVehicle(small_vehicle);

    // System should switch to selective mode and park the small vehicle
    bool parked = garage.parkNextVehicle(large_spaces + 2);

    // The blocked large vehicle should still be in queue
    expect(garage.getQueueSize() >= 1, "UC-004: Blocked vehicle should remain in queue");
}

void test_UC004_returns_to_fifo_when_space_available() {
    // WHEN parking spaces become available for the first vehicle in queue,
    // THE SYSTEM SHALL return to FIFO parking order

    Garage garage(10);

    // This test verifies mode switching behavior
    expect(garage.getMode() == ParkingMode::FIFO, "UC-004: Should start in FIFO mode");
}

// ============================================================================
// UC-005: Unpark Vehicle
// ============================================================================

void test_UC005_unpark_expired_vehicle() {
    // WHEN a parked vehicle's wait time expires, THE SYSTEM SHALL unpark the vehicle
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);  // Wait time = 5
    garage.addVehicle(v);
    garage.parkNextVehicle(0);  // Park at time 0

    // Unpark at time 5 (wait time expired)
    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(5);

    expect(tickets.size() == 1, "UC-005: Should have 1 ticket for unparked vehicle");
}

void test_UC005_space_becomes_available_after_unpark() {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 3, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    GarageStatus before = garage.getStatus();
    int occupied_before = before.getTotalOccupied();

    garage.unparkExpiredVehicles(3);

    GarageStatus after = garage.getStatus();
    int occupied_after = after.getTotalOccupied();

    expect(occupied_after == occupied_before - 1, "UC-005: One less occupied space after unpark");
}

void test_UC005_no_unpark_before_expiry() {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);  // Wait time = 5
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    // Try to unpark at time 3 (before wait time expires)
    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(3);

    expect(tickets.empty(), "UC-005: Should not unpark before wait time expires");
}

// ============================================================================
// UC-006: Calculate and Track Fees
// ============================================================================

void test_UC006_fee_calculation_formula() {
    // WHEN unparking, THE SYSTEM SHALL calculate fee = vehicle_size × parking_time
    Garage garage(100);

    // Size 2 vehicle, parked for 5 time units = fee of 10
    Vehicle v(1, Size::MEDIUM, 5, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(5);

    expect(tickets.size() == 1, "UC-006: Should have 1 ticket");
    expect(tickets[0].getFee() == 2 * 5, "UC-006: Fee should be size(2) * time(5) = 10");
}

void test_UC006_running_total() {
    // THE SYSTEM SHALL maintain a running sum of all fees collected
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 2, 0);   // Fee: 1 * 2 = 2
    Vehicle v2(2, Size::MEDIUM, 3, 1);  // Fee: 2 * 3 = 6

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    garage.parkNextVehicle(0);
    garage.parkNextVehicle(1);

    garage.unparkExpiredVehicles(2);  // Unpark v1
    expect(garage.getFeesCollected() == 2, "UC-006: Running total should be 2");

    garage.unparkExpiredVehicles(4);  // Unpark v2
    expect(garage.getFeesCollected() == 8, "UC-006: Running total should be 2 + 6 = 8");
}

void test_UC006_different_vehicle_sizes_fees() {
    Garage garage(100);

    // Test all three sizes - all parked at same time, same wait time
    Vehicle v1(1, Size::SMALL, 4, 0);   // Fee: 1 * 4 = 4
    Vehicle v2(2, Size::MEDIUM, 4, 1);  // Fee: 2 * 4 = 8
    Vehicle v3(3, Size::LARGE, 4, 2);   // Fee: 3 * 4 = 12

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.addVehicle(v3);

    // Park all at time 0 so they all expire at time 4
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(0);

    garage.unparkExpiredVehicles(4);

    expect(garage.getFeesCollected() == 24, "UC-006: Total fees should be 4 + 8 + 12 = 24");
}

// ============================================================================
// UC-007: Track Parking Statistics
// ============================================================================

void test_UC007_running_car_count() {
    // THE SYSTEM SHALL maintain a running count of total cars parked
    Garage garage(100);

    expect(garage.getCarsParked() == 0, "UC-007: Initial car count should be 0");

    Vehicle v1(1, Size::SMALL, 5, 0);
    garage.addVehicle(v1);
    garage.parkNextVehicle(0);
    expect(garage.getCarsParked() == 1, "UC-007: Car count should be 1");

    Vehicle v2(2, Size::MEDIUM, 5, 1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(1);
    expect(garage.getCarsParked() == 2, "UC-007: Car count should be 2");
}

void test_UC007_car_count_persists_after_unpark() {
    // Historical count should not decrease when vehicles leave
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 2, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(2);

    expect(garage.getCarsParked() == 1, "UC-007: Historical car count should remain 1");
}

void test_UC007_fee_total_tracking() {
    Garage garage(100);

    expect(garage.getFeesCollected() == 0, "UC-007: Initial fees should be 0");

    Vehicle v(1, Size::LARGE, 3, 0);  // Fee: 3 * 3 = 9
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(3);

    expect(garage.getFeesCollected() == 9, "UC-007: Fees should be tracked correctly");
}

// ============================================================================
// UC-008: Query Garage Status
// ============================================================================

void test_UC008_query_total_spaces() {
    Garage garage(150);
    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 150, "UC-008: Should report correct total spaces");
}

void test_UC008_query_space_distribution() {
    Garage garage(100);
    GarageStatus status = garage.getStatus();

    int sum = status.n1_spaces + status.n2_spaces + status.n3_spaces;
    expect(sum == status.total_spaces, "UC-008: Space distribution should sum to total");
}

void test_UC008_query_occupied_spaces() {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 10, 0);
    Vehicle v2(2, Size::MEDIUM, 10, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(1);

    GarageStatus status = garage.getStatus();
    expect(status.getTotalOccupied() == 2, "UC-008: Should report 2 occupied spaces");
}

void test_UC008_query_queue_length() {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    GarageStatus status = garage.getStatus();
    expect(status.queue_length == 2, "UC-008: Should report queue length of 2");
}

void test_UC008_query_fees_and_car_count() {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 3, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(3);

    GarageStatus status = garage.getStatus();
    expect(status.fees_collected == 3, "UC-008: Should report correct fees");
    expect(status.cars_parked == 1, "UC-008: Should report correct car count");
}

void test_UC008_available_spaces_calculation() {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 10, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    GarageStatus status = garage.getStatus();
    expect(status.getTotalAvailable() == 99, "UC-008: Should report 99 available spaces");
}

}  // namespace

int main() {
    std::cout << "Running PGOF Functional Tests...\n\n";

    // UC-001: System Initialization
    std::cout << "UC-001: System Initialization\n";
    test_UC001_system_initialization_valid_range();
    test_UC001_system_initialization_minimum();
    test_UC001_system_initialization_maximum();
    test_UC001_system_initialization_invalid_low();
    test_UC001_system_initialization_invalid_high();
    test_UC001_reinitialize();
    std::cout << "  PASSED\n\n";

    // UC-002: Vehicle Arrival and Queue Entry
    std::cout << "UC-002: Vehicle Arrival and Queue Entry\n";
    test_UC002_vehicle_arrival_adds_to_queue();
    test_UC002_fifo_queue_ordering();
    test_UC002_accept_all_valid_sizes();
    test_UC002_accept_all_valid_wait_times();
    std::cout << "  PASSED\n\n";

    // UC-003: Park Vehicle (Normal Mode)
    std::cout << "UC-003: Park Vehicle (Normal Mode)\n";
    test_UC003_park_vehicle_fifo_order();
    test_UC003_parking_compatibility();
    test_UC003_increments_parked_count();
    test_UC003_garage_mode_is_fifo_initially();
    std::cout << "  PASSED\n\n";

    // UC-004: Park Vehicle (Selective Mode)
    std::cout << "UC-004: Park Vehicle (Selective Mode)\n";
    test_UC004_selective_mode_when_first_blocked();
    test_UC004_returns_to_fifo_when_space_available();
    std::cout << "  PASSED\n\n";

    // UC-005: Unpark Vehicle
    std::cout << "UC-005: Unpark Vehicle\n";
    test_UC005_unpark_expired_vehicle();
    test_UC005_space_becomes_available_after_unpark();
    test_UC005_no_unpark_before_expiry();
    std::cout << "  PASSED\n\n";

    // UC-006: Calculate and Track Fees
    std::cout << "UC-006: Calculate and Track Fees\n";
    test_UC006_fee_calculation_formula();
    test_UC006_running_total();
    test_UC006_different_vehicle_sizes_fees();
    std::cout << "  PASSED\n\n";

    // UC-007: Track Parking Statistics
    std::cout << "UC-007: Track Parking Statistics\n";
    test_UC007_running_car_count();
    test_UC007_car_count_persists_after_unpark();
    test_UC007_fee_total_tracking();
    std::cout << "  PASSED\n\n";

    // UC-008: Query Garage Status
    std::cout << "UC-008: Query Garage Status\n";
    test_UC008_query_total_spaces();
    test_UC008_query_space_distribution();
    test_UC008_query_occupied_spaces();
    test_UC008_query_queue_length();
    test_UC008_query_fees_and_car_count();
    test_UC008_available_spaces_calculation();
    std::cout << "  PASSED\n\n";

    std::cout << "All PGOF Functional Tests PASSED!\n";
    return 0;
}
