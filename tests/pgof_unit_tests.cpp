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

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>

using namespace pgof;

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "Test failed: " << message << '\n';
        std::exit(EXIT_FAILURE);
    }
}

// ============================================================================
// Vehicle Class Unit Tests
// ============================================================================

void test_vehicle_constructor() {
    Vehicle v(42, Size::MEDIUM, 7, 100);

    expect(v.getId() == 42, "Vehicle: ID should be 42");
    expect(v.getSize() == Size::MEDIUM, "Vehicle: Size should be MEDIUM");
    expect(v.getRequestedWaitTime() == 7, "Vehicle: Wait time should be 7");
    expect(v.getArrivalTime() == 100, "Vehicle: Arrival time should be 100");
}

void test_vehicle_getId() {
    Vehicle v(123, Size::SMALL, 5, 0);
    expect(v.getId() == 123, "Vehicle::getId should return correct ID");
}

void test_vehicle_getSize() {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 0);
    Vehicle v3(3, Size::LARGE, 5, 0);

    expect(v1.getSize() == Size::SMALL, "Vehicle::getSize should return SMALL");
    expect(v2.getSize() == Size::MEDIUM, "Vehicle::getSize should return MEDIUM");
    expect(v3.getSize() == Size::LARGE, "Vehicle::getSize should return LARGE");
}

void test_vehicle_getRequestedWaitTime() {
    Vehicle v(1, Size::SMALL, 10, 0);
    expect(v.getRequestedWaitTime() == 10, "Vehicle::getRequestedWaitTime should return 10");
}

void test_vehicle_getArrivalTime() {
    Vehicle v(1, Size::SMALL, 5, 42);
    expect(v.getArrivalTime() == 42, "Vehicle::getArrivalTime should return 42");
}

void test_vehicle_canFitIn_small_vehicle() {
    Vehicle v(1, Size::SMALL, 5, 0);

    expect(v.canFitIn(Size::SMALL), "Small vehicle can fit in small space");
    expect(v.canFitIn(Size::MEDIUM), "Small vehicle can fit in medium space");
    expect(v.canFitIn(Size::LARGE), "Small vehicle can fit in large space");
}

void test_vehicle_canFitIn_medium_vehicle() {
    Vehicle v(1, Size::MEDIUM, 5, 0);

    expect(!v.canFitIn(Size::SMALL), "Medium vehicle cannot fit in small space");
    expect(v.canFitIn(Size::MEDIUM), "Medium vehicle can fit in medium space");
    expect(v.canFitIn(Size::LARGE), "Medium vehicle can fit in large space");
}

void test_vehicle_canFitIn_large_vehicle() {
    Vehicle v(1, Size::LARGE, 5, 0);

    expect(!v.canFitIn(Size::SMALL), "Large vehicle cannot fit in small space");
    expect(!v.canFitIn(Size::MEDIUM), "Large vehicle cannot fit in medium space");
    expect(v.canFitIn(Size::LARGE), "Large vehicle can fit in large space");
}

void test_vehicle_equality_operator() {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(1, Size::SMALL, 5, 0);
    Vehicle v3(2, Size::SMALL, 5, 0);

    expect(v1 == v2, "Vehicles with same attributes should be equal");
    expect(!(v1 == v3), "Vehicles with different IDs should not be equal");
}

void test_vehicle_inequality_operator() {
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 0);

    expect(v1 != v2, "Vehicles with different IDs should be not equal");
}

// ============================================================================
// ParkingSpace Class Unit Tests
// ============================================================================

void test_parking_space_constructor() {
    ParkingSpace space(10, Size::MEDIUM);

    expect(space.getId() == 10, "ParkingSpace: ID should be 10");
    expect(space.getSize() == Size::MEDIUM, "ParkingSpace: Size should be MEDIUM");
    expect(!space.isOccupied(), "ParkingSpace: Should be initially unoccupied");
}

void test_parking_space_getId() {
    ParkingSpace space(99, Size::LARGE);
    expect(space.getId() == 99, "ParkingSpace::getId should return correct ID");
}

void test_parking_space_getSize() {
    ParkingSpace s1(1, Size::SMALL);
    ParkingSpace s2(2, Size::MEDIUM);
    ParkingSpace s3(3, Size::LARGE);

    expect(s1.getSize() == Size::SMALL, "ParkingSpace::getSize should return SMALL");
    expect(s2.getSize() == Size::MEDIUM, "ParkingSpace::getSize should return MEDIUM");
    expect(s3.getSize() == Size::LARGE, "ParkingSpace::getSize should return LARGE");
}

void test_parking_space_isOccupied() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    expect(!space.isOccupied(), "ParkingSpace should be initially unoccupied");

    space.park(v, 0);
    expect(space.isOccupied(), "ParkingSpace should be occupied after parking");

    space.unpark();
    expect(!space.isOccupied(), "ParkingSpace should be unoccupied after unparking");
}

void test_parking_space_getParkedVehicle() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(42, Size::SMALL, 5, 0);

    expect(space.getParkedVehicle() == nullptr, "Should return nullptr when empty");

    space.park(v, 0);
    const Vehicle* parked = space.getParkedVehicle();
    expect(parked != nullptr, "Should return pointer to parked vehicle");
    expect(parked->getId() == 42, "Parked vehicle should have correct ID");
}

void test_parking_space_getParkingStartTime() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    space.park(v, 100);
    expect(space.getParkingStartTime() == 100, "ParkingSpace::getParkingStartTime should return 100");
}

void test_parking_space_canAccommodate_empty_space() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v_small(1, Size::SMALL, 5, 0);
    Vehicle v_medium(2, Size::MEDIUM, 5, 0);
    Vehicle v_large(3, Size::LARGE, 5, 0);

    expect(space.canAccommodate(v_small), "Medium space can accommodate small vehicle");
    expect(space.canAccommodate(v_medium), "Medium space can accommodate medium vehicle");
    expect(!space.canAccommodate(v_large), "Medium space cannot accommodate large vehicle");
}

void test_parking_space_canAccommodate_occupied_space() {
    ParkingSpace space(1, Size::LARGE);
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 0);

    space.park(v1, 0);
    expect(!space.canAccommodate(v2), "Occupied space cannot accommodate any vehicle");
}

void test_parking_space_park() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(1, Size::SMALL, 5, 0);

    bool result = space.park(v, 10);
    expect(result, "park() should return true on success");
    expect(space.isOccupied(), "Space should be occupied after parking");
    expect(space.getParkingStartTime() == 10, "Start time should be recorded");
}

void test_parking_space_park_incompatible() {
    ParkingSpace space(1, Size::SMALL);
    Vehicle v(1, Size::LARGE, 5, 0);

    bool result = space.park(v, 0);
    expect(!result, "park() should return false for incompatible vehicle");
    expect(!space.isOccupied(), "Space should remain empty");
}

void test_parking_space_park_already_occupied() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::SMALL, 5, 1);

    space.park(v1, 0);
    bool result = space.park(v2, 1);
    expect(!result, "park() should return false when space is occupied");
}

void test_parking_space_unpark() {
    ParkingSpace space(1, Size::MEDIUM);
    Vehicle v(42, Size::SMALL, 5, 0);

    space.park(v, 0);
    auto unparked = space.unpark();

    expect(unparked.has_value(), "unpark() should return vehicle");
    expect(unparked->getId() == 42, "Unparked vehicle should have correct ID");
    expect(!space.isOccupied(), "Space should be empty after unparking");
}

void test_parking_space_unpark_empty() {
    ParkingSpace space(1, Size::MEDIUM);

    auto unparked = space.unpark();
    expect(!unparked.has_value(), "unpark() on empty space should return nullopt");
}

// ============================================================================
// ParkingTicket Class Unit Tests
// ============================================================================

void test_parking_ticket_constructor() {
    ParkingTicket ticket(1, 100, Size::MEDIUM, 50, 10, 20);

    expect(ticket.getTicketId() == 1, "ParkingTicket: ID should be 1");
    expect(ticket.getVehicleId() == 100, "ParkingTicket: Vehicle ID should be 100");
    expect(ticket.getSpaceId() == 50, "ParkingTicket: Space ID should be 50");
    expect(ticket.getStartTime() == 10, "ParkingTicket: Start time should be 10");
    expect(ticket.getEndTime() == 20, "ParkingTicket: End time should be 20");
}

void test_parking_ticket_getTicketId() {
    ParkingTicket ticket(99, 1, Size::SMALL, 1, 0, 5);
    expect(ticket.getTicketId() == 99, "ParkingTicket::getTicketId should return 99");
}

void test_parking_ticket_getVehicleId() {
    ParkingTicket ticket(1, 42, Size::SMALL, 1, 0, 5);
    expect(ticket.getVehicleId() == 42, "ParkingTicket::getVehicleId should return 42");
}

void test_parking_ticket_getSpaceId() {
    ParkingTicket ticket(1, 1, Size::SMALL, 77, 0, 5);
    expect(ticket.getSpaceId() == 77, "ParkingTicket::getSpaceId should return 77");
}

void test_parking_ticket_getStartTime() {
    ParkingTicket ticket(1, 1, Size::SMALL, 1, 15, 25);
    expect(ticket.getStartTime() == 15, "ParkingTicket::getStartTime should return 15");
}

void test_parking_ticket_getEndTime() {
    ParkingTicket ticket(1, 1, Size::SMALL, 1, 15, 25);
    expect(ticket.getEndTime() == 25, "ParkingTicket::getEndTime should return 25");
}

void test_parking_ticket_calculateFee() {
    // Fee = vehicle_size × parking_time
    // Size::SMALL = 1, time = 5, fee = 5
    ParkingTicket ticket_small(1, 1, Size::SMALL, 1, 0, 5);
    expect(ticket_small.calculateFee() == 5, "Fee for SMALL vehicle, 5 time units should be 5");

    // Size::MEDIUM = 2, time = 4, fee = 8
    ParkingTicket ticket_medium(2, 2, Size::MEDIUM, 2, 0, 4);
    expect(ticket_medium.calculateFee() == 8, "Fee for MEDIUM vehicle, 4 time units should be 8");

    // Size::LARGE = 3, time = 3, fee = 9
    ParkingTicket ticket_large(3, 3, Size::LARGE, 3, 0, 3);
    expect(ticket_large.calculateFee() == 9, "Fee for LARGE vehicle, 3 time units should be 9");
}

void test_parking_ticket_getFee() {
    ParkingTicket ticket(1, 1, Size::MEDIUM, 1, 10, 15);  // Fee = 2 * 5 = 10
    expect(ticket.getFee() == 10, "ParkingTicket::getFee should return pre-calculated fee");
}

// ============================================================================
// WaitQueue Class Unit Tests
// ============================================================================

void test_wait_queue_enqueue() {
    WaitQueue queue;
    Vehicle v(1, Size::SMALL, 5, 0);

    queue.enqueue(v);
    expect(queue.size() == 1, "WaitQueue::enqueue should add vehicle to queue");
}

void test_wait_queue_dequeue() {
    WaitQueue queue;
    Vehicle v(42, Size::MEDIUM, 5, 0);

    queue.enqueue(v);
    Vehicle dequeued = queue.dequeue();

    expect(dequeued.getId() == 42, "WaitQueue::dequeue should return correct vehicle");
    expect(queue.isEmpty(), "Queue should be empty after dequeue");
}

void test_wait_queue_dequeue_empty() {
    WaitQueue queue;

    bool threw = false;
    try {
        queue.dequeue();
    } catch (const std::runtime_error&) {
        threw = true;
    }
    expect(threw, "WaitQueue::dequeue on empty queue should throw");
}

void test_wait_queue_front() {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle& front = queue.front();
    expect(front.getId() == 1, "WaitQueue::front should return first vehicle");
    expect(queue.size() == 2, "front() should not remove the vehicle");
}

void test_wait_queue_front_empty() {
    WaitQueue queue;

    bool threw = false;
    try {
        queue.front();
    } catch (const std::runtime_error&) {
        threw = true;
    }
    expect(threw, "WaitQueue::front on empty queue should throw");
}

void test_wait_queue_findFirstEligible() {
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

    expect(eligible != nullptr, "Should find an eligible vehicle");
    expect(eligible->getId() == 2, "Should find the small vehicle as eligible");
}

void test_wait_queue_findFirstEligible_none_found() {
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    // Only small space available
    spaces.emplace_back(1, Size::SMALL);

    // All large vehicles
    Vehicle v(1, Size::LARGE, 5, 0);
    queue.enqueue(v);

    const Vehicle* eligible = queue.findFirstEligible(spaces);
    expect(eligible == nullptr, "Should return nullptr when no vehicle can fit");
}

void test_wait_queue_findFirstEligible_occupied_spaces() {
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    // All spaces occupied
    spaces.emplace_back(1, Size::LARGE);
    Vehicle occupant(99, Size::SMALL, 10, 0);
    spaces[0].park(occupant, 0);

    Vehicle v(1, Size::SMALL, 5, 1);
    queue.enqueue(v);

    const Vehicle* eligible = queue.findFirstEligible(spaces);
    expect(eligible == nullptr, "Should return nullptr when all spaces occupied");
}

void test_wait_queue_remove() {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);
    Vehicle v3(3, Size::LARGE, 5, 2);

    queue.enqueue(v1);
    queue.enqueue(v2);
    queue.enqueue(v3);

    bool removed = queue.remove(v2);

    expect(removed, "WaitQueue::remove should return true when vehicle found");
    expect(queue.size() == 2, "Queue should have 2 vehicles after removal");
}

void test_wait_queue_remove_not_found() {
    WaitQueue queue;
    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v_other(99, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);

    bool removed = queue.remove(v_other);
    expect(!removed, "WaitQueue::remove should return false when vehicle not found");
    expect(queue.size() == 1, "Queue size should be unchanged");
}

void test_wait_queue_size() {
    WaitQueue queue;

    expect(queue.size() == 0, "Empty queue size should be 0");

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    expect(queue.size() == 1, "Queue size should be 1");

    queue.enqueue(Vehicle(2, Size::SMALL, 5, 1));
    expect(queue.size() == 2, "Queue size should be 2");
}

void test_wait_queue_isEmpty() {
    WaitQueue queue;

    expect(queue.isEmpty(), "New queue should be empty");

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    expect(!queue.isEmpty(), "Queue with vehicle should not be empty");

    queue.dequeue();
    expect(queue.isEmpty(), "Queue after dequeue should be empty");
}

void test_wait_queue_fifo_order() {
    WaitQueue queue;

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    queue.enqueue(Vehicle(2, Size::MEDIUM, 5, 1));
    queue.enqueue(Vehicle(3, Size::LARGE, 5, 2));

    expect(queue.dequeue().getId() == 1, "First dequeue should return first enqueued");
    expect(queue.dequeue().getId() == 2, "Second dequeue should return second enqueued");
    expect(queue.dequeue().getId() == 3, "Third dequeue should return third enqueued");
}

void test_wait_queue_iterators() {
    WaitQueue queue;

    queue.enqueue(Vehicle(1, Size::SMALL, 5, 0));
    queue.enqueue(Vehicle(2, Size::MEDIUM, 5, 1));

    int count = 0;
    for (auto it = queue.begin(); it != queue.end(); ++it) {
        ++count;
    }

    expect(count == 2, "Iterator should traverse all elements");
}

// ============================================================================
// Garage Class Unit Tests
// ============================================================================

void test_garage_constructor() {
    Garage garage(100);
    expect(garage.getTotalSpaces() == 100, "Garage::constructor should set total spaces");
    expect(garage.getFeesCollected() == 0, "Garage should have 0 fees initially");
    expect(garage.getCarsParked() == 0, "Garage should have 0 cars parked initially");
}

void test_garage_constructor_invalid() {
    bool threw_low = false;
    try {
        Garage garage(5);
    } catch (const std::out_of_range&) {
        threw_low = true;
    }
    expect(threw_low, "Garage constructor should throw for N < 10");

    bool threw_high = false;
    try {
        Garage garage(2000000);
    } catch (const std::out_of_range&) {
        threw_high = true;
    }
    expect(threw_high, "Garage constructor should throw for N > 1000000");
}

void test_garage_initialize() {
    Garage garage(100);
    garage.initialize(200);

    expect(garage.getTotalSpaces() == 200, "Garage::initialize should update total spaces");
}

void test_garage_initialize_invalid() {
    Garage garage(100);

    bool threw = false;
    try {
        garage.initialize(5);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    expect(threw, "Garage::initialize should throw for invalid N");
}

void test_garage_addVehicle() {
    Garage garage(100);
    Vehicle v(1, Size::SMALL, 5, 0);

    garage.addVehicle(v);
    expect(garage.getQueueSize() == 1, "Garage::addVehicle should add to queue");
}

void test_garage_parkNextVehicle() {
    Garage garage(100);
    Vehicle v(1, Size::SMALL, 5, 0);

    garage.addVehicle(v);
    bool parked = garage.parkNextVehicle(0);

    expect(parked, "Garage::parkNextVehicle should return true on success");
    expect(garage.getQueueSize() == 0, "Vehicle should be removed from queue");
    expect(garage.getCarsParked() == 1, "Cars parked count should increase");
}

void test_garage_parkNextVehicle_empty_queue() {
    Garage garage(100);
    bool parked = garage.parkNextVehicle(0);

    expect(!parked, "Garage::parkNextVehicle should return false for empty queue");
}

void test_garage_unparkExpiredVehicles() {
    Garage garage(100);
    Vehicle v(1, Size::MEDIUM, 3, 0);  // Wait time = 3

    garage.addVehicle(v);
    garage.parkNextVehicle(0);

    auto tickets = garage.unparkExpiredVehicles(3);

    expect(tickets.size() == 1, "Should unpark 1 vehicle");
    expect(tickets[0].getFee() == 6, "Fee should be 2 * 3 = 6");
}

void test_garage_unparkExpiredVehicles_multiple() {
    Garage garage(100);

    Vehicle v1(1, Size::SMALL, 2, 0);
    Vehicle v2(2, Size::MEDIUM, 2, 1);

    garage.addVehicle(v1);
    garage.addVehicle(v2);
    garage.parkNextVehicle(0);
    garage.parkNextVehicle(0);

    auto tickets = garage.unparkExpiredVehicles(2);
    expect(tickets.size() == 2, "Should unpark 2 vehicles");
}

void test_garage_getFeesCollected() {
    Garage garage(100);
    expect(garage.getFeesCollected() == 0, "Initial fees should be 0");

    Vehicle v(1, Size::LARGE, 4, 0);  // Fee = 3 * 4 = 12
    garage.addVehicle(v);
    garage.parkNextVehicle(0);
    garage.unparkExpiredVehicles(4);

    expect(garage.getFeesCollected() == 12, "Fees should be collected after unpark");
}

void test_garage_getCarsParked() {
    Garage garage(100);

    expect(garage.getCarsParked() == 0, "Initial cars parked should be 0");

    garage.addVehicle(Vehicle(1, Size::SMALL, 5, 0));
    garage.parkNextVehicle(0);
    expect(garage.getCarsParked() == 1, "Cars parked should be 1");
}

void test_garage_getStatus() {
    Garage garage(100);

    GarageStatus status = garage.getStatus();

    expect(status.total_spaces == 100, "Status should report correct total spaces");
    expect(status.fees_collected == 0, "Status should report 0 fees initially");
    expect(status.cars_parked == 0, "Status should report 0 cars initially");
}

void test_garage_findAvailableSpace() {
    Garage garage(100);

    ParkingSpace* space = garage.findAvailableSpace(Size::SMALL);
    expect(space != nullptr, "Should find available space for small vehicle");

    space = garage.findAvailableSpace(Size::LARGE);
    expect(space != nullptr, "Should find available space for large vehicle");
}

void test_garage_setStrategy() {
    Garage garage(100);

    garage.setStrategy(std::make_unique<SelectiveStrategy>());
    // Strategy change should succeed without error
    expect(true, "Garage::setStrategy should accept new strategy");
}

void test_garage_getMode() {
    Garage garage(100);
    expect(garage.getMode() == ParkingMode::FIFO, "Initial mode should be FIFO");
}

void test_garage_getQueueSize() {
    Garage garage(100);

    expect(garage.getQueueSize() == 0, "Initial queue size should be 0");

    garage.addVehicle(Vehicle(1, Size::SMALL, 5, 0));
    expect(garage.getQueueSize() == 1, "Queue size should be 1");
}

void test_garage_getTotalSpaces() {
    Garage garage(250);
    expect(garage.getTotalSpaces() == 250, "Garage::getTotalSpaces should return 250");
}

// ============================================================================
// FIFOStrategy Class Unit Tests
// ============================================================================

void test_fifo_strategy_selectVehicle() {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::MEDIUM);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);

    expect(selected != nullptr, "FIFOStrategy should select a vehicle");
    expect(selected->getId() == 1, "FIFOStrategy should select first vehicle in queue");
}

void test_fifo_strategy_selectVehicle_no_fit() {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);  // Only small space

    Vehicle v(1, Size::LARGE, 5, 0);  // Large vehicle
    queue.enqueue(v);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    expect(selected == nullptr, "FIFOStrategy should return nullptr when first vehicle can't fit");
}

void test_fifo_strategy_selectVehicle_empty_queue() {
    FIFOStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    expect(selected == nullptr, "FIFOStrategy should return nullptr for empty queue");
}

void test_fifo_strategy_selectSpace() {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);
    spaces.emplace_back(2, Size::MEDIUM);
    spaces.emplace_back(3, Size::LARGE);

    Vehicle v(1, Size::MEDIUM, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);

    expect(selected != nullptr, "FIFOStrategy should find a space");
    expect(static_cast<int>(selected->getSize()) >= static_cast<int>(Size::MEDIUM),
           "Selected space should be large enough");
}

void test_fifo_strategy_selectSpace_no_fit() {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    expect(selected == nullptr, "FIFOStrategy should return nullptr when no space fits");
}

void test_fifo_strategy_selectSpace_all_occupied() {
    FIFOStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);
    spaces[0].park(Vehicle(99, Size::SMALL, 10, 0), 0);

    Vehicle v(1, Size::SMALL, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    expect(selected == nullptr, "FIFOStrategy should return nullptr when all spaces occupied");
}

// ============================================================================
// SelectiveStrategy Class Unit Tests
// ============================================================================

void test_selective_strategy_selectVehicle() {
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

    expect(selected != nullptr, "SelectiveStrategy should find an eligible vehicle");
    expect(selected->getId() == 2, "SelectiveStrategy should select first eligible (small) vehicle");
}

void test_selective_strategy_selectVehicle_first_fits() {
    SelectiveStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::LARGE);

    Vehicle v1(1, Size::SMALL, 5, 0);
    Vehicle v2(2, Size::MEDIUM, 5, 1);

    queue.enqueue(v1);
    queue.enqueue(v2);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);

    expect(selected != nullptr, "SelectiveStrategy should find a vehicle");
    expect(selected->getId() == 1, "When first vehicle fits, should select it");
}

void test_selective_strategy_selectVehicle_none_fit() {
    SelectiveStrategy strategy;
    WaitQueue queue;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);
    queue.enqueue(v);

    const Vehicle* selected = strategy.selectVehicle(queue, spaces);
    expect(selected == nullptr, "SelectiveStrategy should return nullptr when none fit");
}

void test_selective_strategy_selectSpace() {
    SelectiveStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);
    spaces.emplace_back(2, Size::MEDIUM);

    Vehicle v(1, Size::SMALL, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);

    expect(selected != nullptr, "SelectiveStrategy should find a space");
}

void test_selective_strategy_selectSpace_no_fit() {
    SelectiveStrategy strategy;
    std::vector<ParkingSpace> spaces;

    spaces.emplace_back(1, Size::SMALL);

    Vehicle v(1, Size::LARGE, 5, 0);

    ParkingSpace* selected = strategy.selectSpace(v, spaces);
    expect(selected == nullptr, "SelectiveStrategy should return nullptr when no space fits");
}

// ============================================================================
// GarageStatus Struct Unit Tests
// ============================================================================

void test_garage_status_getTotalOccupied() {
    GarageStatus status;
    status.occupied_n1 = 5;
    status.occupied_n2 = 3;
    status.occupied_n3 = 2;

    expect(status.getTotalOccupied() == 10, "GarageStatus::getTotalOccupied should sum occupied");
}

void test_garage_status_getTotalAvailable() {
    GarageStatus status;
    status.total_spaces = 100;
    status.occupied_n1 = 10;
    status.occupied_n2 = 5;
    status.occupied_n3 = 5;

    expect(status.getTotalAvailable() == 80, "GarageStatus::getTotalAvailable should be total - occupied");
}

void test_garage_status_default_values() {
    GarageStatus status;

    expect(status.total_spaces == 0, "Default total_spaces should be 0");
    expect(status.n1_spaces == 0, "Default n1_spaces should be 0");
    expect(status.n2_spaces == 0, "Default n2_spaces should be 0");
    expect(status.n3_spaces == 0, "Default n3_spaces should be 0");
    expect(status.occupied_n1 == 0, "Default occupied_n1 should be 0");
    expect(status.occupied_n2 == 0, "Default occupied_n2 should be 0");
    expect(status.occupied_n3 == 0, "Default occupied_n3 should be 0");
    expect(status.queue_length == 0, "Default queue_length should be 0");
    expect(status.fees_collected == 0, "Default fees_collected should be 0");
    expect(status.cars_parked == 0, "Default cars_parked should be 0");
}

// ============================================================================
// types.h Utility Tests
// ============================================================================

void test_canFit_function() {
    // Small vehicle can fit in any space
    expect(canFit(Size::SMALL, Size::SMALL), "Small fits in small");
    expect(canFit(Size::SMALL, Size::MEDIUM), "Small fits in medium");
    expect(canFit(Size::SMALL, Size::LARGE), "Small fits in large");

    // Medium vehicle fits in medium or large
    expect(!canFit(Size::MEDIUM, Size::SMALL), "Medium doesn't fit in small");
    expect(canFit(Size::MEDIUM, Size::MEDIUM), "Medium fits in medium");
    expect(canFit(Size::MEDIUM, Size::LARGE), "Medium fits in large");

    // Large vehicle fits only in large
    expect(!canFit(Size::LARGE, Size::SMALL), "Large doesn't fit in small");
    expect(!canFit(Size::LARGE, Size::MEDIUM), "Large doesn't fit in medium");
    expect(canFit(Size::LARGE, Size::LARGE), "Large fits in large");
}

void test_size_enum_values() {
    expect(static_cast<int>(Size::SMALL) == 1, "Size::SMALL should be 1");
    expect(static_cast<int>(Size::MEDIUM) == 2, "Size::MEDIUM should be 2");
    expect(static_cast<int>(Size::LARGE) == 3, "Size::LARGE should be 3");
}

}  // namespace

int main() {
    std::cout << "Running PGOF Unit Tests...\n\n";

    // Vehicle Tests
    std::cout << "Vehicle Class Tests\n";
    test_vehicle_constructor();
    test_vehicle_getId();
    test_vehicle_getSize();
    test_vehicle_getRequestedWaitTime();
    test_vehicle_getArrivalTime();
    test_vehicle_canFitIn_small_vehicle();
    test_vehicle_canFitIn_medium_vehicle();
    test_vehicle_canFitIn_large_vehicle();
    test_vehicle_equality_operator();
    test_vehicle_inequality_operator();
    std::cout << "  PASSED\n\n";

    // ParkingSpace Tests
    std::cout << "ParkingSpace Class Tests\n";
    test_parking_space_constructor();
    test_parking_space_getId();
    test_parking_space_getSize();
    test_parking_space_isOccupied();
    test_parking_space_getParkedVehicle();
    test_parking_space_getParkingStartTime();
    test_parking_space_canAccommodate_empty_space();
    test_parking_space_canAccommodate_occupied_space();
    test_parking_space_park();
    test_parking_space_park_incompatible();
    test_parking_space_park_already_occupied();
    test_parking_space_unpark();
    test_parking_space_unpark_empty();
    std::cout << "  PASSED\n\n";

    // ParkingTicket Tests
    std::cout << "ParkingTicket Class Tests\n";
    test_parking_ticket_constructor();
    test_parking_ticket_getTicketId();
    test_parking_ticket_getVehicleId();
    test_parking_ticket_getSpaceId();
    test_parking_ticket_getStartTime();
    test_parking_ticket_getEndTime();
    test_parking_ticket_calculateFee();
    test_parking_ticket_getFee();
    std::cout << "  PASSED\n\n";

    // WaitQueue Tests
    std::cout << "WaitQueue Class Tests\n";
    test_wait_queue_enqueue();
    test_wait_queue_dequeue();
    test_wait_queue_dequeue_empty();
    test_wait_queue_front();
    test_wait_queue_front_empty();
    test_wait_queue_findFirstEligible();
    test_wait_queue_findFirstEligible_none_found();
    test_wait_queue_findFirstEligible_occupied_spaces();
    test_wait_queue_remove();
    test_wait_queue_remove_not_found();
    test_wait_queue_size();
    test_wait_queue_isEmpty();
    test_wait_queue_fifo_order();
    test_wait_queue_iterators();
    std::cout << "  PASSED\n\n";

    // Garage Tests
    std::cout << "Garage Class Tests\n";
    test_garage_constructor();
    test_garage_constructor_invalid();
    test_garage_initialize();
    test_garage_initialize_invalid();
    test_garage_addVehicle();
    test_garage_parkNextVehicle();
    test_garage_parkNextVehicle_empty_queue();
    test_garage_unparkExpiredVehicles();
    test_garage_unparkExpiredVehicles_multiple();
    test_garage_getFeesCollected();
    test_garage_getCarsParked();
    test_garage_getStatus();
    test_garage_findAvailableSpace();
    test_garage_setStrategy();
    test_garage_getMode();
    test_garage_getQueueSize();
    test_garage_getTotalSpaces();
    std::cout << "  PASSED\n\n";

    // FIFOStrategy Tests
    std::cout << "FIFOStrategy Class Tests\n";
    test_fifo_strategy_selectVehicle();
    test_fifo_strategy_selectVehicle_no_fit();
    test_fifo_strategy_selectVehicle_empty_queue();
    test_fifo_strategy_selectSpace();
    test_fifo_strategy_selectSpace_no_fit();
    test_fifo_strategy_selectSpace_all_occupied();
    std::cout << "  PASSED\n\n";

    // SelectiveStrategy Tests
    std::cout << "SelectiveStrategy Class Tests\n";
    test_selective_strategy_selectVehicle();
    test_selective_strategy_selectVehicle_first_fits();
    test_selective_strategy_selectVehicle_none_fit();
    test_selective_strategy_selectSpace();
    test_selective_strategy_selectSpace_no_fit();
    std::cout << "  PASSED\n\n";

    // GarageStatus Tests
    std::cout << "GarageStatus Struct Tests\n";
    test_garage_status_getTotalOccupied();
    test_garage_status_getTotalAvailable();
    test_garage_status_default_values();
    std::cout << "  PASSED\n\n";

    // types.h Tests
    std::cout << "types.h Utility Tests\n";
    test_canFit_function();
    test_size_enum_values();
    std::cout << "  PASSED\n\n";

    std::cout << "All PGOF Unit Tests PASSED!\n";
    return 0;
}
