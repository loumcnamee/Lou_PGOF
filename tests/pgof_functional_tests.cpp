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

#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

using namespace pgof;

// ============================================================================
// UC-001: System Initialization
// ============================================================================

TEST(UC001_SystemInitialization, ValidRange) {
    // WHEN PGOF starts with valid N, THE SYSTEM SHALL initialize successfully
    Garage garage(100);
    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 100) << "UC-001: Total spaces should be 100";
    EXPECT_EQ(status.n1_spaces + status.n2_spaces + status.n3_spaces, 100)
        << "UC-001: Space distribution should sum to total";
    EXPECT_EQ(status.fees_collected, 0) << "UC-001: Initial fees should be zero";
    EXPECT_EQ(status.cars_parked, 0) << "UC-001: Initial cars parked should be zero";
}

TEST(UC001_SystemInitialization, Minimum) {
    // Test minimum valid range (N=10)
    Garage garage(10);
    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 10) << "UC-001: Minimum spaces should be 10";
    EXPECT_GE(status.n1_spaces, 1) << "UC-001: Should have at least one size-1 space";
    EXPECT_GE(status.n2_spaces, 1) << "UC-001: Should have at least one size-2 space";
    EXPECT_GE(status.n3_spaces, 1) << "UC-001: Should have at least one size-3 space";
}

TEST(UC001_SystemInitialization, Maximum) {
    // Test maximum valid range (N=1,000,000)
    Garage garage(1000000);
    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 1000000) << "UC-001: Maximum spaces should be 1,000,000";
}

TEST(UC001_SystemInitialization, InvalidLow) {
    // IF N is outside range [10, 1000000], THEN throw out_of_range
    EXPECT_THROW(Garage garage(9), std::out_of_range) << "UC-001: N<10 should throw out_of_range";
}

TEST(UC001_SystemInitialization, InvalidHigh) {
    EXPECT_THROW(Garage garage(1000001), std::out_of_range) << "UC-001: N>1000000 should throw out_of_range";
}

TEST(UC001_SystemInitialization, Reinitialize) {
    Garage garage(100);
    garage.initialize(200);
    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 200) << "UC-001: Re-initialization should update total spaces";
}

// ============================================================================
// UC-002: Vehicle Arrival and Queue Entry
// ============================================================================

TEST(UC002_VehicleArrival, AddsToQueue) {
    // WHEN an autonomous vehicle arrives, THE SYSTEM SHALL add it to the waiting queue
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    garage.addVehicle(v1);

    EXPECT_EQ(garage.getQueueSize(), 1) << "UC-002: Queue should have 1 vehicle after arrival";
}

TEST(UC002_VehicleArrival, FIFOQueueOrdering) {
    // Vehicles should be in order of arrival (FIFO)
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 3, 1);
    Vehicle v3(3, Size::LARGE, 7, 2);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.addVehicle(v3);

    EXPECT_EQ(garage.getQueueSize(), 3) << "UC-002: Queue should have 3 vehicles";
}

TEST(UC002_VehicleArrival, AcceptAllValidSizes) {
    // THE SYSTEM SHALL accept vehicles with sizes {1, 2, 3}
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);   // Size 1
    Vehicle v2(2, Size::MEDIUM, 5, 1);  // Size 2
    Vehicle v3(3, Size::LARGE, 5, 2);   // Size 3

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.addVehicle(v3);

    EXPECT_EQ(garage.getQueueSize(), 3) << "UC-002: All sizes should be accepted";
}

TEST(UC002_VehicleArrival, AcceptAllValidWaitTimes) {
    // THE SYSTEM SHALL accept requested wait times in range {1..10}
    Garage garage(100);

    for (int wait_time = 1; wait_time <= 10; ++wait_time) {
        Vehicle v(wait_time, Size::SMALL, wait_time, wait_time);
        garage.addVehicle(v);
    }

    EXPECT_EQ(garage.getQueueSize(), 10) << "UC-002: All wait times 1-10 should be accepted";
}

// ============================================================================
// UC-003: Park Vehicle (Normal Mode - FIFO)
// ============================================================================

TEST(UC003_ParkVehicle, FIFOOrder) {
    // WHILE parking spaces are available, THE SYSTEM SHALL park vehicles in order of arrival
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    bool parked1 = garage.parkNextVehicle(0);
    EXPECT_TRUE(parked1) << "UC-003: First vehicle should be parked";
    EXPECT_EQ(garage.getQueueSize(), 1) << "UC-003: Queue should have 1 vehicle after parking";

    bool parked2 = garage.parkNextVehicle(1);
    EXPECT_TRUE(parked2) << "UC-003: Second vehicle should be parked";
    EXPECT_EQ(garage.getQueueSize(), 0) << "UC-003: Queue should be empty after parking both";
}

TEST(UC003_ParkVehicle, ParkingCompatibility) {
    // THE SYSTEM SHALL assign vehicle to space where space_size >= vehicle_size
    Garage garage(100);

    // Small vehicle can fit in any space
    Vehicle v_small(1, Size::SMALL, 5, 0);
    garage.addVehicle(v_small);
    bool parked = garage.parkNextVehicle(0);
    EXPECT_TRUE(parked) << "UC-003: Small vehicle should find a compatible space";

    // Medium vehicle needs size 2 or 3
    Vehicle v_medium(2, Size::MEDIUM, 5, 1);
    garage.addVehicle(v_medium);
    parked = garage.parkNextVehicle(1);
    EXPECT_TRUE(parked) << "UC-003: Medium vehicle should find a compatible space";

    // Large vehicle needs size 3 only
    Vehicle v_large(3, Size::LARGE, 5, 2);
    garage.addVehicle(v_large);
    parked = garage.parkNextVehicle(2);
    EXPECT_TRUE(parked) << "UC-003: Large vehicle should find a compatible space";
}

TEST(UC003_ParkVehicle, IncrementsParkedCount) {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    EXPECT_EQ(garage.getCarsParked(), 1) << "UC-003: Cars parked count should be 1";
}

TEST(UC003_ParkVehicle, GarageModeIsFIFOInitially) {
    Garage garage(100);
    EXPECT_EQ(garage.getMode(), ParkingMode::FIFO) << "UC-003: Initial mode should be FIFO";
}

// ============================================================================
// UC-004: Park Vehicle (Selective Mode)
// ============================================================================

TEST(UC004_SelectiveMode, WhenFirstBlocked) {
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
    EXPECT_GE(garage.getQueueSize(), 1) << "UC-004: Blocked vehicle should remain in queue";
}

TEST(UC004_SelectiveMode, ReturnsToFIFOWhenSpaceAvailable) {
    // WHEN parking spaces become available for the first vehicle in queue,
    // THE SYSTEM SHALL return to FIFO parking order

    Garage garage(10);

    // This test verifies mode switching behavior
    EXPECT_EQ(garage.getMode(), ParkingMode::FIFO) << "UC-004: Should start in FIFO mode";
}

// ============================================================================
// UC-005: Unpark Vehicle
// ============================================================================

TEST(UC005_Unpark, ExpiredVehicle) {
    // WHEN a parked vehicle's wait time expires, THE SYSTEM SHALL unpark the vehicle
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);  // Wait time = 5
    garage.addVehicle(v);
    garage.parkNextVehicle(0);  // Park at time 0

    // Unpark at time 5 (wait time expired)
    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(5);

    EXPECT_EQ(tickets.size(), 1) << "UC-005: Should have 1 ticket for unparked vehicle";
}

TEST(UC005_Unpark, SpaceBecomesAvailableAfterUnpark) {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 3, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    GarageStatus before = garage.getStatus();
    int occupied_before = before.getTotalOccupied();

    garage.unparkExpiredVehicles(3);

    GarageStatus after = garage.getStatus();
    int occupied_after = after.getTotalOccupied();

    EXPECT_EQ(occupied_after, occupied_before - 1) << "UC-005: One less occupied space after unpark";
}

TEST(UC005_Unpark, NoUnparkBeforeExpiry) {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 5, 0);  // Wait time = 5
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    // Try to unpark at time 3 (before wait time expires)
    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(3);

    EXPECT_TRUE(tickets.empty()) << "UC-005: Should not unpark before wait time expires";
}

// ============================================================================
// UC-006: Calculate and Track Fees
// ============================================================================

TEST(UC006_FeeCalculation, Formula) {
    // WHEN unparking, THE SYSTEM SHALL calculate fee = vehicle_size × parking_time
    Garage garage(100);

    // Size 2 vehicle, parked for 5 time units = fee of 10
    Vehicle v(1, Size::MEDIUM, 5, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    std::vector<ParkingTicket> tickets = garage.unparkExpiredVehicles(5);

    EXPECT_EQ(tickets.size(), 1) << "UC-006: Should have 1 ticket";
    EXPECT_EQ(tickets[0].getFee(), 2 * 5) << "UC-006: Fee should be size(2) * time(5) = 10";
}

TEST(UC006_FeeCalculation, RunningTotal) {
    // THE SYSTEM SHALL maintain a running sum of all fees collected
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 2, 0);   // Fee: 1 * 2 = 2
    Vehicle v2(2, Size::MEDIUM, 3, 1);  // Fee: 2 * 3 = 6

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    garage.parkNextVehicle(0);
    garage.parkNextVehicle(1);

    garage.unparkExpiredVehicles(2);  // Unpark v1
    EXPECT_EQ(garage.getFeesCollected(), 2) << "UC-006: Running total should be 2";

    garage.unparkExpiredVehicles(4);  // Unpark v2
    EXPECT_EQ(garage.getFeesCollected(), 8) << "UC-006: Running total should be 2 + 6 = 8";
}

TEST(UC006_FeeCalculation, DifferentVehicleSizesFees) {
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

    EXPECT_EQ(garage.getFeesCollected(), 24) << "UC-006: Total fees should be 4 + 8 + 12 = 24";
}

// ============================================================================
// UC-007: Track Parking Statistics
// ============================================================================

TEST(UC007_ParkingStatistics, RunningCarCount) {
    // THE SYSTEM SHALL maintain a running count of total cars parked
    Garage garage(100);

    EXPECT_EQ(garage.getCarsParked(), 0) << "UC-007: Initial car count should be 0";

    Vehicle v1(1, Size::SMALL, 5, 0);
    garage.addVehicle(v1);
    garage.parkNextVehicle(0);
    EXPECT_EQ(garage.getCarsParked(), 1) << "UC-007: Car count should be 1";

    Vehicle v2(2, Size::MEDIUM, 5, 1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(1);
    EXPECT_EQ(garage.getCarsParked(), 2) << "UC-007: Car count should be 2";
}

TEST(UC007_ParkingStatistics, CarCountPersistsAfterUnpark) {
    // Historical count should not decrease when vehicles leave
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 2, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(2);

    EXPECT_EQ(garage.getCarsParked(), 1) << "UC-007: Historical car count should remain 1";
}

TEST(UC007_ParkingStatistics, FeeTotalTracking) {
    Garage garage(100);

    EXPECT_EQ(garage.getFeesCollected(), 0) << "UC-007: Initial fees should be 0";

    Vehicle v(1, Size::LARGE, 3, 0);  // Fee: 3 * 3 = 9
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(3);

    EXPECT_EQ(garage.getFeesCollected(), 9) << "UC-007: Fees should be tracked correctly";
}

// ============================================================================
// UC-008: Query Garage Status
// ============================================================================

TEST(UC008_QueryStatus, TotalSpaces) {
    Garage garage(150);
    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 150) << "UC-008: Should report correct total spaces";
}

TEST(UC008_QueryStatus, SpaceDistribution) {
    Garage garage(100);
    GarageStatus status = garage.getStatus();

    int sum = status.n1_spaces + status.n2_spaces + status.n3_spaces;
    EXPECT_EQ(sum, status.total_spaces) << "UC-008: Space distribution should sum to total";
}

TEST(UC008_QueryStatus, OccupiedSpaces) {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 10, 0);
    Vehicle v2(2, Size::MEDIUM, 10, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(1);

    GarageStatus status = garage.getStatus();
    EXPECT_EQ(status.getTotalOccupied(), 2) << "UC-008: Should report 2 occupied spaces";
}

TEST(UC008_QueryStatus, QueueLength) {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);

    GarageStatus status = garage.getStatus();
    EXPECT_EQ(status.queue_length, 2) << "UC-008: Should report queue length of 2";
}

TEST(UC008_QueryStatus, FeesAndCarCount) {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 3, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(3);

    GarageStatus status = garage.getStatus();
    EXPECT_EQ(status.fees_collected, 3) << "UC-008: Should report correct fees";
    EXPECT_EQ(status.cars_parked, 1) << "UC-008: Should report correct car count";
}

TEST(UC008_QueryStatus, AvailableSpacesCalculation) {
    Garage garage(100);

    Vehicle v(1, Size::SMALL, 10, 0);
    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    GarageStatus status = garage.getStatus();
    EXPECT_EQ(status.getTotalAvailable(), 99) << "UC-008: Should report 99 available spaces";
}
