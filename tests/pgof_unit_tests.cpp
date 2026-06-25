/**
 * @file pgof_unit_tests.cpp
 * @brief Unit tests for all PGOF library classes and their public methods.
 *
 * Tests cover:
 * - Vehicle class
 * - ParkingSpace class
 * - ParkingTicket class
 * - WaitQueue class
 * - Garage class
 * - FIFOStrategy class
 * - SelectiveStrategy class
 * - GarageStatus struct
 * - types.h utilities
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
#include <memory>

using namespace pgof;

// ============================================================================
// Vehicle Class Unit Tests
// ============================================================================

TEST(VehicleTest, Constructor) {
    Vehicle v(42, Size::MEDIUM, 7, 100);

    EXPECT_EQ(v.getId(), 42) << "Vehicle: ID should be 42";
    EXPECT_EQ(v.getSize(), Size::MEDIUM) << "Vehicle: Size should be MEDIUM";
    EXPECT_EQ(v.getRequestedWaitTime(), 7) << "Vehicle: Wait time should be 7";
    EXPECT_EQ(v.getArrivalTime(), 100) << "Vehicle: Arrival time should be 100";
}

TEST(VehicleTest, GetId) {
    Vehicle v(123, Size::SMALL, 5, 0);
    EXPECT_EQ(v.getId(), 123) << "Vehicle::getId should return correct ID";
}

TEST(VehicleTest, GetSize) {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 0);
    Vehicle v3(3, Size::LARGE, 5, 0);

    EXPECT_EQ(v1.getSize(), Size::SMALL) << "Vehicle::getSize should return SMALL";
    EXPECT_EQ(v2.getSize(), Size::MEDIUM) << "Vehicle::getSize should return MEDIUM";
    EXPECT_EQ(v3.getSize(), Size::LARGE) << "Vehicle::getSize should return LARGE";
}

TEST(VehicleTest, GetRequestedWaitTime) {
    Vehicle v(1, Size::SMALL, 10, 0);
    EXPECT_EQ(v.getRequestedWaitTime(), 10) << "Vehicle::getRequestedWaitTime should return 10";
}

TEST(VehicleTest, GetArrivalTime) {
    Vehicle v(1, Size::SMALL, 5, 42);
    EXPECT_EQ(v.getArrivalTime(), 42) << "Vehicle::getArrivalTime should return 42";
}

TEST(VehicleTest, CanFitInSmallVehicle) {
    Vehicle v(1, Size::SMALL, 5, 0);

    EXPECT_TRUE(v.canFitIn(Size::SMALL)) << "Small vehicle can fit in small space";
    EXPECT_TRUE(v.canFitIn(Size::MEDIUM)) << "Small vehicle can fit in medium space";
    EXPECT_TRUE(v.canFitIn(Size::LARGE)) << "Small vehicle can fit in large space";
}

TEST(VehicleTest, CanFitInMediumVehicle) {
    Vehicle v(1, Size::MEDIUM, 5, 0);

    EXPECT_FALSE(v.canFitIn(Size::SMALL)) << "Medium vehicle cannot fit in small space";
    EXPECT_TRUE(v.canFitIn(Size::MEDIUM)) << "Medium vehicle can fit in medium space";
    EXPECT_TRUE(v.canFitIn(Size::LARGE)) << "Medium vehicle can fit in large space";
}

TEST(VehicleTest, CanFitInLargeVehicle) {
    Vehicle v(1, Size::LARGE, 5, 0);

    EXPECT_FALSE(v.canFitIn(Size::SMALL)) << "Large vehicle cannot fit in small space";
    EXPECT_FALSE(v.canFitIn(Size::MEDIUM)) << "Large vehicle cannot fit in medium space";
    EXPECT_TRUE(v.canFitIn(Size::LARGE)) << "Large vehicle can fit in large space";
}

TEST(VehicleTest, EqualityOperator) {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(1, Size::SMALL, 5, 0);
    Vehicle v3(2, Size::SMALL, 5, 0);

    EXPECT_TRUE(v1 == v2) << "Vehicles with same attributes should be equal";
    EXPECT_FALSE(v1 == v3) << "Vehicles with different IDs should not be equal";
}

TEST(VehicleTest, InequalityOperator) {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 0);

    EXPECT_TRUE(v1 != v2) << "Vehicles with different IDs should be not equal";
}

// ============================================================================
// ParkingSpace Class Unit Tests
// ============================================================================

TEST(ParkingSpaceTest, Constructor) {
    ParkingSpace space(10, Size::MEDIUM);

    EXPECT_EQ(space.getId(), 10) << "ParkingSpace: ID should be 10";
    EXPECT_EQ(space.getSize(), Size::MEDIUM) << "ParkingSpace: Size should be MEDIUM";
    EXPECT_FALSE(space.isOccupied()) << "ParkingSpace: Should be initially unoccupied";
}

TEST(ParkingSpaceTest, GetId) {
    ParkingSpace space(99, Size::LARGE);
    EXPECT_EQ(space.getId(), 99) << "ParkingSpace::getId should return correct ID";
}

TEST(ParkingSpaceTest, GetSize) {
    ParkingSpace s1(1, Size::SMALL);
    ParkingSpace s2(2, Size::MEDIUM);
    ParkingSpace s3(3, Size::LARGE);

    EXPECT_EQ(s1.getSize(), Size::SMALL) << "ParkingSpace::getSize should return SMALL";
    EXPECT_EQ(s2.getSize(), Size::MEDIUM) << "ParkingSpace::getSize should return MEDIUM";
    EXPECT_EQ(s3.getSize(), Size::LARGE) << "ParkingSpace::getSize should return LARGE";
}

TEST(ParkingSpaceTest, IsOccupied) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    EXPECT_FALSE(space.isOccupied()) << "ParkingSpace should be initially unoccupied";

    space.park(v, 0);
    EXPECT_TRUE(space.isOccupied()) << "ParkingSpace should be occupied after parking";

    space.unpark();
    EXPECT_FALSE(space.isOccupied()) << "ParkingSpace should be unoccupied after unparking";
}

TEST(ParkingSpaceTest, GetParkedVehicle) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(42, Size::SMALL, 5, 0);

    EXPECT_EQ(space.getParkedVehicle(), nullptr) << "Should return nullptr when empty";

    space.park(v, 0);
    const Vehicle* parked = space.getParkedVehicle();
    EXPECT_NE(parked, nullptr) << "Should return pointer to parked vehicle";
    EXPECT_EQ(parked->getId(), 42) << "Parked vehicle should have correct ID";
}

TEST(ParkingSpaceTest, GetParkingStartTime) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    space.park(v, 100);
    EXPECT_EQ(space.getParkingStartTime(), 100) << "ParkingSpace::getParkingStartTime should return 100";
}

TEST(ParkingSpaceTest, CanAccommodateEmptySpace) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v_small(1, Size::SMALL, 5, 0);
    Vehicle v_medium(2, Size::MEDIUM, 5, 0);
    Vehicle v_large(3, Size::LARGE, 5, 0);

    EXPECT_TRUE(space.canAccommodate(v_small)) << "Medium space can accommodate small vehicle";
    EXPECT_TRUE(space.canAccommodate(v_medium)) << "Medium space can accommodate medium vehicle";
    EXPECT_FALSE(space.canAccommodate(v_large)) << "Medium space cannot accommodate large vehicle";
}

TEST(ParkingSpaceTest, CanAccommodateOccupiedSpace) {
    ParkingSpace space(1, Size::LARGE);
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 0);

    space.park(v1, 0);
    EXPECT_FALSE(space.canAccommodate(v2)) << "Occupied space cannot accommodate any vehicle";
}

TEST(ParkingSpaceTest, Park) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    bool result = space.park(v, 10);
    EXPECT_TRUE(result) << "park() should return true on success";
    EXPECT_TRUE(space.isOccupied()) << "Space should be occupied after parking";
    EXPECT_EQ(space.getParkingStartTime(), 10) << "Start time should be recorded";
}

TEST(ParkingSpaceTest, ParkIncompatible) {
    ParkingSpace space(1, Size::SMALL);
    Vehicle v(1, Size::LARGE, 5, 0);

    bool result = space.park(v, 0);
    EXPECT_FALSE(result) << "park() should return false for incompatible vehicle";
    EXPECT_FALSE(space.isOccupied()) << "Space should remain empty";
}

TEST(ParkingSpaceTest, ParkAlreadyOccupied) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 1);

    space.park(v1, 0);
    bool result = space.park(v2, 1);
    EXPECT_FALSE(result) << "park() should return false when space is occupied";
}

TEST(ParkingSpaceTest, Unpark) {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(42, Size::SMALL, 5, 0);

    space.park(v, 0);
    auto unparked = space.unpark();

    EXPECT_TRUE(unparked.has_value()) << "unpark() should return vehicle";
    EXPECT_EQ(unparked->getId(), 42) << "Unparked vehicle should have correct ID";
    EXPECT_FALSE(space.isOccupied()) << "Space should be empty after unparking";
}

TEST(ParkingSpaceTest, UnparkEmpty) {
    ParkingSpace space(1, Size::MEDIUM);

    auto unparked = space.unpark();
    EXPECT_FALSE(unparked.has_value()) << "unpark() on empty space should return nullopt";
}

// ============================================================================
// ParkingTicket Class Unit Tests
// ============================================================================

TEST(ParkingTicketTest, Constructor) {
    ParkingTicket ticket(1, 100, Size::MEDIUM, 50, 10, 20);

    EXPECT_EQ(ticket.getTicketId(), 1) << "ParkingTicket: ID should be 1";
    EXPECT_EQ(ticket.getVehicleId(), 100) << "ParkingTicket: Vehicle ID should be 100";
    EXPECT_EQ(ticket.getSpaceId(), 50) << "ParkingTicket: Space ID should be 50";
    EXPECT_EQ(ticket.getStartTime(), 10) << "ParkingTicket: Start time should be 10";
    EXPECT_EQ(ticket.getEndTime(), 20) << "ParkingTicket: End time should be 20";
}

TEST(ParkingTicketTest, GetTicketId) {
    ParkingTicket ticket(99, 1, Size::SMALL, 1, 0, 5);
    EXPECT_EQ(ticket.getTicketId(), 99) << "ParkingTicket::getTicketId should return 99";
}

TEST(ParkingTicketTest, GetVehicleId) {
    ParkingTicket ticket(1, 42, Size::SMALL, 1, 0, 5);
    EXPECT_EQ(ticket.getVehicleId(), 42) << "ParkingTicket::getVehicleId should return 42";
}

TEST(ParkingTicketTest, GetSpaceId) {
    ParkingTicket ticket(1, 1, Size::SMALL, 77, 0, 5);
    EXPECT_EQ(ticket.getSpaceId(), 77) << "ParkingTicket::getSpaceId should return 77";
}

TEST(ParkingTicketTest, GetStartTime) {
    ParkingTicket ticket(1, 1, Size::SMALL, 1, 15, 25);
    EXPECT_EQ(ticket.getStartTime(), 15) << "ParkingTicket::getStartTime should return 15";
}

TEST(ParkingTicketTest, GetEndTime) {
    ParkingTicket ticket(1, 1, Size::SMALL, 1, 15, 25);
    EXPECT_EQ(ticket.getEndTime(), 25) << "ParkingTicket::getEndTime should return 25";
}

TEST(ParkingTicketTest, CalculateFee) {
    // Fee = vehicle_size × parking_time
    // Size::SMALL = 1, time = 5, fee = 5
    ParkingTicket ticket_small(1, 1, Size::SMALL, 1, 0, 5);
    EXPECT_EQ(ticket_small.calculateFee(), 5) << "Fee for SMALL vehicle, 5 time units should be 5";

    // Size::MEDIUM = 2, time = 4, fee = 8
    ParkingTicket ticket_medium(2, 2, Size::MEDIUM, 2, 0, 4);
    EXPECT_EQ(ticket_medium.calculateFee(), 8) << "Fee for MEDIUM vehicle, 4 time units should be 8";

    // Size::LARGE = 3, time = 3, fee = 9
    ParkingTicket ticket_large(3, 3, Size::LARGE, 3, 0, 3);
    EXPECT_EQ(ticket_large.calculateFee(), 9) << "Fee for LARGE vehicle, 3 time units should be 9";
}

TEST(ParkingTicketTest, GetFee) {
    ParkingTicket ticket(1, 1, Size::MEDIUM, 1, 10, 15);  // Fee = 2 * 5 = 10
    EXPECT_EQ(ticket.getFee(), 10) << "ParkingTicket::getFee should return pre-calculated fee";
}

// ============================================================================
// WaitQueue Class Unit Tests
// ============================================================================

TEST(WaitQueueTest, Enqueue) {
    WaitQueue queue;
    Vehicle v(1, Size::SMALL, 5, 0);

    queue.enqueue(v);
    EXPECT_EQ(queue.size(), 1) << "WaitQueue::enqueue should add vehicle to queue";
}

TEST(WaitQueueTest, Dequeue) {
    WaitQueue queue;
    Vehicle v(42, Size::MEDIUM, 5, 0);

    queue.enqueue(v);
    Vehicle dequeued = queue.dequeue();

    EXPECT_EQ(dequeued.getId(), 42) << "WaitQueue::dequeue should return correct vehicle";
    EXPECT_TRUE(queue.isEmpty()) << "Queue should be empty after dequeue";
}

TEST(WaitQueueTest, DequeueEmpty) {
    WaitQueue queue;

    EXPECT_THROW(queue.dequeue(), std::runtime_error) << "WaitQueue::dequeue on empty queue should throw";
}

TEST(WaitQueueTest, Front) {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle& front = queue.front();
    EXPECT_EQ(front.getId(), 1) << "WaitQueue::front should return first vehicle";
    EXPECT_EQ(queue.size(), 2) << "front() should not remove the vehicle";
}

TEST(WaitQueueTest, FrontEmpty) {
    WaitQueue queue;

    EXPECT_THROW(queue.front(), std::runtime_error) << "WaitQueue::front on empty queue should throw";
}

TEST(WaitQueueTest, FindFirstEligible) {
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    // Only size-1 space available
    spaces.emplace_back(1, Size::SMALL);

    // Add large vehicle (can't fit), then small vehicle (can fit)
    Vehicle v_large(1, Size::LARGE, 5, 0);
    Vehicle v_small(2, Size::SMALL, 5, 1);

    queue.enqueue(v_large);
    queue.enqueue(v_small);

    const Vehicle* eligible = queue.findFirstEligible(spaces);

    EXPECT_NE(eligible, nullptr) << "Should find an eligible vehicle";
    EXPECT_EQ(eligible->getId(), 2) << "Should find the small vehicle as eligible";
}

TEST(WaitQueueTest, FindFirstEligibleNoneFound) {
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    // Only small space available
    spaces.emplace_back(1, Size::SMALL);

    // All large vehicles
    Vehicle v(1, Size::LARGE, 5, 0);
    queue.enqueue(v);

    const Vehicle* eligible = queue.findFirstEligible(spaces);
    EXPECT_EQ(eligible, nullptr) << "Should return nullptr when no vehicle can fit";
}

TEST(WaitQueueTest, FindFirstEligibleOccupiedSpaces) {
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    // All spaces occupied
    spaces.emplace_back(1, Size::LARGE);
    Vehicle occupant(99, Size::SMALL, 10, 0);
    spaces[0].park(occupant, 0);

    Vehicle v(1, Size::SMALL, 5, 1);
    queue.enqueue(v);

    const Vehicle* eligible = queue.findFirstEligible(spaces);
    EXPECT_EQ(eligible, nullptr) << "Should return nullptr when all spaces occupied";
}

TEST(WaitQueueTest, Remove) {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);
    Vehicle v3(3, Size::LARGE, 5, 2);

    queue.enqueue(v1);
    queue.enqueue(v2);
    queue.enqueue(v3);

    bool removed = queue.remove(v2);

    EXPECT_TRUE(removed) << "WaitQueue::remove should return true when vehicle found";
    EXPECT_EQ(queue.size(), 2) << "Queue should have 2 vehicles after removal";
}

TEST(WaitQueueTest, RemoveNotFound) {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v_other(99, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);

    bool removed = queue.remove(v_other);
    EXPECT_FALSE(removed) << "WaitQueue::remove should return false when vehicle not found";
    EXPECT_EQ(queue.size(), 1) << "Queue size should be unchanged";
}

TEST(WaitQueueTest, Size) {
    WaitQueue queue;

    EXPECT_EQ(queue.size(), 0) << "Empty queue size should be 0";

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    EXPECT_EQ(queue.size(), 1) << "Queue size should be 1";

    queue.enqueue(Vehicle(2, Size::SMALL, 5, 1));
    EXPECT_EQ(queue.size(), 2) << "Queue size should be 2";
}

TEST(WaitQueueTest, IsEmpty) {
    WaitQueue queue;

    EXPECT_TRUE(queue.isEmpty()) << "New queue should be empty";

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    EXPECT_FALSE(queue.isEmpty()) << "Queue with vehicle should not be empty";

    queue.dequeue();
    EXPECT_TRUE(queue.isEmpty()) << "Queue after dequeue should be empty";
}

TEST(WaitQueueTest, FIFOOrder) {
    WaitQueue queue;

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    queue.enqueue(Vehicle(2, Size::MEDIUM, 5, 1));
    queue.enqueue(Vehicle(3, Size::LARGE, 5, 2));

    EXPECT_EQ(queue.dequeue().getId(), 1) << "First dequeue should return first enqueued";
    EXPECT_EQ(queue.dequeue().getId(), 2) << "Second dequeue should return second enqueued";
    EXPECT_EQ(queue.dequeue().getId(), 3) << "Third dequeue should return third enqueued";
}

TEST(WaitQueueTest, Iterators) {
    WaitQueue queue;

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    queue.enqueue(Vehicle(2, Size::MEDIUM, 5, 1));

    int count = 0;
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        ++count;
    }

    EXPECT_EQ(count, 2) << "Iterator should traverse all elements";
}

// ============================================================================
// Garage Class Unit Tests
// ============================================================================

TEST(GarageTest, Constructor) {
    Garage garage(100);
    EXPECT_EQ(garage.getTotalSpaces(), 100) << "Garage::constructor should set total spaces";
    EXPECT_EQ(garage.getFeesCollected(), 0) << "Garage should have 0 fees initially";
    EXPECT_EQ(garage.getCarsParked(), 0) << "Garage should have 0 cars parked initially";
}

TEST(GarageTest, ConstructorInvalid) {
    EXPECT_THROW(Garage garage(5), std::out_of_range) << "Garage constructor should throw for N < 10";
    EXPECT_THROW(Garage garage(2000000), std::out_of_range) << "Garage constructor should throw for N > 1000000";
}

TEST(GarageTest, Initialize) {
    Garage garage(100);
    garage.initialize(200);

    EXPECT_EQ(garage.getTotalSpaces(), 200) << "Garage::initialize should update total spaces";
}

TEST(GarageTest, InitializeInvalid) {
    Garage garage(100);

    EXPECT_THROW(garage.initialize(5), std::out_of_range) << "Garage::initialize should throw for invalid N";
}

TEST(GarageTest, AddVehicle) {
    Garage garage(100);
    Vehicle v(1, Size::SMALL, 5, 0);

    garage.addVehicle(v);
    EXPECT_EQ(garage.getQueueSize(), 1) << "Garage::addVehicle should add to queue";
}

TEST(GarageTest, ParkNextVehicle) {
    Garage garage(100);
    Vehicle v(1, Size::SMALL, 5, 0);

    garage.addVehicle(v);
    bool parked = garage.parkNextVehicle(0);

    EXPECT_TRUE(parked) << "Garage::parkNextVehicle should return true on success";
    EXPECT_EQ(garage.getQueueSize(), 0) << "Vehicle should be removed from queue";
    EXPECT_EQ(garage.getCarsParked(), 1) << "Cars parked count should increase";
}

TEST(GarageTest, ParkNextVehicleEmptyQueue) {
    Garage garage(100);
    bool parked = garage.parkNextVehicle(0);

    EXPECT_FALSE(parked) << "Garage::parkNextVehicle should return false for empty queue";
}

TEST(GarageTest, UnparkExpiredVehicles) {
    Garage garage(100);
    Vehicle v(1, Size::MEDIUM, 3, 0);  // Wait time = 3

    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    auto tickets = garage.unparkExpiredVehicles(3);

    EXPECT_EQ(tickets.size(), 1) << "Should unpark 1 vehicle";
    EXPECT_EQ(tickets[0].getFee(), 6) << "Fee should be 2 * 3 = 6";
}

TEST(GarageTest, UnparkExpiredVehiclesMultiple) {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 2, 0);
    Vehicle v2(2, Size::MEDIUM, 2, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(0);

    auto tickets = garage.unparkExpiredVehicles(2);
    EXPECT_EQ(tickets.size(), 2) << "Should unpark 2 vehicles";
}

TEST(GarageTest, GetFeesCollected) {
    Garage garage(100);
    EXPECT_EQ(garage.getFeesCollected(), 0) << "Initial fees should be 0";

    Vehicle v(1, Size::LARGE, 4, 0);  // Fee = 3 * 4 = 12
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(4);

    EXPECT_EQ(garage.getFeesCollected(), 12) << "Fees should be collected after unpark";
}

TEST(GarageTest, GetCarsParked) {
    Garage garage(100);

    EXPECT_EQ(garage.getCarsParked(), 0) << "Initial cars parked should be 0";

    garage.addVehicle(Vehicle(1, Size::SMALL, 5, 0));
    garage.parkNextVehicle(0);
    EXPECT_EQ(garage.getCarsParked(), 1) << "Cars parked should be 1";
}

TEST(GarageTest, GetStatus) {
    Garage garage(100);

    GarageStatus status = garage.getStatus();

    EXPECT_EQ(status.total_spaces, 100) << "Status should report correct total spaces";
    EXPECT_EQ(status.fees_collected, 0) << "Status should report 0 fees initially";
    EXPECT_EQ(status.cars_parked, 0) << "Status should report 0 cars initially";
}

TEST(GarageTest, FindAvailableSpace) {
    Garage garage(100);

    ParkingSpace* space = garage.findAvailableSpace(Size::SMALL);
    EXPECT_NE(space, nullptr) << "Should find available space for small vehicle";

    space = garage.findAvailableSpace(Size::LARGE);
    EXPECT_NE(space, nullptr) << "Should find available space for large vehicle";
}

TEST(GarageTest, SetStrategy) {
    Garage garage(100);

    garage.setStrategy(std::make_unique<SelectiveStrategy>());
    // Strategy change should succeed without error
    SUCCEED() << "Garage::setStrategy should accept new strategy";
}

TEST(GarageTest, GetMode) {
    Garage garage(100);
    EXPECT_EQ(garage.getMode(), ParkingMode::FIFO) << "Initial mode should be FIFO";
}

TEST(GarageTest, GetQueueSize) {
    Garage garage(100);

    EXPECT_EQ(garage.getQueueSize(), 0) << "Initial queue size should be 0";

    garage.addVehicle(Vehicle(1, Size::SMALL, 5, 0));
    EXPECT_EQ(garage.getQueueSize(), 1) << "Queue size should be 1";
}

TEST(GarageTest, GetTotalSpaces) {
    Garage garage(250);
    EXPECT_EQ(garage.getTotalSpaces(), 250) << "Garage::getTotalSpaces should return 250";
}

// ============================================================================
// FIFOStrategy Class Unit Tests
// ============================================================================

TEST(FIFOStrategyTest, SelectVehicle) {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::MEDIUM);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);

    EXPECT_NE(selected, nullptr) << "FIFOStrategy should select a vehicle";
    EXPECT_EQ(selected->getId(), 1) << "FIFOStrategy should select first vehicle in queue";
}

TEST(FIFOStrategyTest, SelectVehicleNoFit) {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);  // Only small space

    Vehicle v(1, Size::LARGE, 5, 0);  // Large vehicle
    queue.enqueue(v);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    EXPECT_EQ(selected, nullptr) << "FIFOStrategy should return nullptr when first vehicle can't fit";
}

TEST(FIFOStrategyTest, SelectVehicleEmptyQueue) {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    EXPECT_EQ(selected, nullptr) << "FIFOStrategy should return nullptr for empty queue";
}

TEST(FIFOStrategyTest, SelectSpace) {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);
    spaces.emplace_back(2, Size::MEDIUM);
    spaces.emplace_back(3, Size::LARGE);

    Vehicle v(1, Size::MEDIUM, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);

    EXPECT_NE(selected, nullptr) << "FIFOStrategy should find a space";
    EXPECT_GE(static_cast<int>(selected->getSize()), static_cast<int>(Size::MEDIUM))
        << "Selected space should be large enough";
}

TEST(FIFOStrategyTest, SelectSpaceNoFit) {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    EXPECT_EQ(selected, nullptr) << "FIFOStrategy should return nullptr when no space fits";
}

TEST(FIFOStrategyTest, SelectSpaceAllOccupied) {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);
    spaces[0].park(Vehicle(99, Size::SMALL, 10, 0), 0);

    Vehicle v(1, Size::SMALL, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    EXPECT_EQ(selected, nullptr) << "FIFOStrategy should return nullptr when all spaces occupied";
}

// ============================================================================
// SelectiveStrategy Class Unit Tests
// ============================================================================

TEST(SelectiveStrategyTest, SelectVehicle) {
    SelectiveStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);  // Only small space

    // Large vehicle first (can't fit), then small vehicle (can fit)
    Vehicle v_large(1, Size::LARGE, 5, 0);
    Vehicle v_small(2, Size::SMALL, 5, 1);

    queue.enqueue(v_large);
    queue.enqueue(v_small);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);

    EXPECT_NE(selected, nullptr) << "SelectiveStrategy should find an eligible vehicle";
    EXPECT_EQ(selected->getId(), 2) << "SelectiveStrategy should select first eligible (small) vehicle";
}

TEST(SelectiveStrategyTest, SelectVehicleFirstFits) {
    SelectiveStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);

    EXPECT_NE(selected, nullptr) << "SelectiveStrategy should find a vehicle";
    EXPECT_EQ(selected->getId(), 1) << "When first vehicle fits, should select it";
}

TEST(SelectiveStrategyTest, SelectVehicleNoneFit) {
    SelectiveStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);
    queue.enqueue(v);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    EXPECT_EQ(selected, nullptr) << "SelectiveStrategy should return nullptr when none fit";
}

TEST(SelectiveStrategyTest, SelectSpace) {
    SelectiveStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);
    spaces.emplace_back(2, Size::MEDIUM);

    Vehicle v(1, Size::SMALL, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);

    EXPECT_NE(selected, nullptr) << "SelectiveStrategy should find a space";
}

TEST(SelectiveStrategyTest, SelectSpaceNoFit) {
    SelectiveStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    EXPECT_EQ(selected, nullptr) << "SelectiveStrategy should return nullptr when no space fits";
}

// ============================================================================
// GarageStatus Struct Unit Tests
// ============================================================================

TEST(GarageStatusTest, GetTotalOccupied) {
    GarageStatus status;
    status.occupied_n1 = 5;
    status.occupied_n2 = 3;
    status.occupied_n3 = 2;

    EXPECT_EQ(status.getTotalOccupied(), 10) << "GarageStatus::getTotalOccupied should sum occupied";
}

TEST(GarageStatusTest, GetTotalAvailable) {
    GarageStatus status;
    status.total_spaces = 100;
    status.occupied_n1 = 10;
    status.occupied_n2 = 5;
    status.occupied_n3 = 5;

    EXPECT_EQ(status.getTotalAvailable(), 80) << "GarageStatus::getTotalAvailable should be total - occupied";
}

TEST(GarageStatusTest, DefaultValues) {
    GarageStatus status;

    EXPECT_EQ(status.total_spaces, 0) << "Default total_spaces should be 0";
    EXPECT_EQ(status.n1_spaces, 0) << "Default n1_spaces should be 0";
    EXPECT_EQ(status.n2_spaces, 0) << "Default n2_spaces should be 0";
    EXPECT_EQ(status.n3_spaces, 0) << "Default n3_spaces should be 0";
    EXPECT_EQ(status.occupied_n1, 0) << "Default occupied_n1 should be 0";
    EXPECT_EQ(status.occupied_n2, 0) << "Default occupied_n2 should be 0";
    EXPECT_EQ(status.occupied_n3, 0) << "Default occupied_n3 should be 0";
    EXPECT_EQ(status.queue_length, 0) << "Default queue_length should be 0";
    EXPECT_EQ(status.fees_collected, 0) << "Default fees_collected should be 0";
    EXPECT_EQ(status.cars_parked, 0) << "Default cars_parked should be 0";
}

// ============================================================================
// types.h Utility Tests
// ============================================================================

TEST(TypesTest, CanFitFunction) {
    // Small vehicle can fit in any space
    EXPECT_TRUE(canFit(Size::SMALL, Size::SMALL)) << "Small fits in small";
    EXPECT_TRUE(canFit(Size::SMALL, Size::MEDIUM)) << "Small fits in medium";
    EXPECT_TRUE(canFit(Size::SMALL, Size::LARGE)) << "Small fits in large";

    // Medium vehicle fits in medium or large
    EXPECT_FALSE(canFit(Size::MEDIUM, Size::SMALL)) << "Medium doesn't fit in small";
    EXPECT_TRUE(canFit(Size::MEDIUM, Size::MEDIUM)) << "Medium fits in medium";
    EXPECT_TRUE(canFit(Size::MEDIUM, Size::LARGE)) << "Medium fits in large";

    // Large vehicle fits only in large
    EXPECT_FALSE(canFit(Size::LARGE, Size::SMALL)) << "Large doesn't fit in small";
    EXPECT_FALSE(canFit(Size::LARGE, Size::MEDIUM)) << "Large doesn't fit in medium";
    EXPECT_TRUE(canFit(Size::LARGE, Size::LARGE)) << "Large fits in large";
}

TEST(TypesTest, SizeEnumValues) {
    EXPECT_EQ(static_cast<int>(Size::SMALL), 1) << "Size::SMALL should be 1";
    EXPECT_EQ(static_cast<int>(Size::MEDIUM), 2) << "Size::MEDIUM should be 2";
    EXPECT_EQ(static_cast<int>(Size::LARGE), 3) << "Size::LARGE should be 3";
}
