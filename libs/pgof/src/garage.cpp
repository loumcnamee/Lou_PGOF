#include "pgof/garage.h"
#include "pgof/fifo_strategy.h"
#include "pgof/selective_strategy.h"
#include "garage_capacity.h"
#include <stdexcept>
#include <algorithm>

namespace pgof {

Garage::Garage(int total_spaces) {
    initialize(total_spaces);
    strategy_ = std::make_unique<FIFOStrategy>();
}

void Garage::initialize(int total_spaces) {
    // Use garage_capacity library to calculate space distribution
    auto capacity = get_garage_capacity(static_cast<unsigned int>(total_spaces));

    n1_spaces_ = capacity.n1;
    n2_spaces_ = capacity.n2;
    n3_spaces_ = capacity.n3;

    createSpaces(n1_spaces_, n2_spaces_, n3_spaces_);

    // Reset statistics
    fees_collected_ = 0;
    cars_parked_ = 0;
    mode_ = ParkingMode::FIFO;
    next_ticket_id_ = 1;

    // Clear the queue
    while (!queue_.isEmpty()) {
        queue_.dequeue();
    }
}

void Garage::createSpaces(int n1, int n2, int n3) {
    spaces_.clear();
    spaces_.reserve(static_cast<size_t>(n1 + n2 + n3));

    int id = 1;

    // Create size-1 spaces
    for (int i = 0; i < n1; ++i) {
        spaces_.emplace_back(id++, Size::SMALL);
    }

    // Create size-2 spaces
    for (int i = 0; i < n2; ++i) {
        spaces_.emplace_back(id++, Size::MEDIUM);
    }

    // Create size-3 spaces
    for (int i = 0; i < n3; ++i) {
        spaces_.emplace_back(id++, Size::LARGE);
    }
}

void Garage::addVehicle(const Vehicle& vehicle) {
    queue_.enqueue(vehicle);
}

bool Garage::parkNextVehicle(int current_time) {
    if (queue_.isEmpty()) {
        return false;
    }

    updateMode();

    // Select vehicle based on current strategy
    const Vehicle* vehicle_to_park = nullptr;

    if (mode_ == ParkingMode::FIFO) {
        vehicle_to_park = strategy_->selectVehicle(queue_, spaces_);
    } else {
        // In SELECTIVE mode, use SelectiveStrategy
        SelectiveStrategy selective;
        vehicle_to_park = selective.selectVehicle(queue_, spaces_);
    }

    if (vehicle_to_park == nullptr) {
        return false;
    }

    // Find a space for the vehicle
    ParkingSpace* space = nullptr;
    if (mode_ == ParkingMode::FIFO) {
        space = strategy_->selectSpace(*vehicle_to_park, spaces_);
    } else {
        SelectiveStrategy selective;
        space = selective.selectSpace(*vehicle_to_park, spaces_);
    }

    if (space == nullptr) {
        return false;
    }

    // Park the vehicle
    if (space->park(*vehicle_to_park, current_time)) {
        queue_.remove(*vehicle_to_park);
        cars_parked_++;
        updateMode();
        return true;
    }

    return false;
}

std::vector<ParkingTicket> Garage::unparkExpiredVehicles(int current_time) {
    std::vector<ParkingTicket> tickets;

    for (auto& space : spaces_) {
        if (space.isOccupied()) {
            const Vehicle* vehicle = space.getParkedVehicle();
            if (vehicle != nullptr) {
                int start_time = space.getParkingStartTime();
                int elapsed = current_time - start_time;

                if (elapsed >= vehicle->getRequestedWaitTime()) {
                    // Time expired, unpark vehicle
                    int space_id = space.getId();
                    Size vehicle_size = vehicle->getSize();
                    int vehicle_id = vehicle->getId();

                    space.unpark();

                    // Create ticket and calculate fee
                    ParkingTicket ticket(
                        next_ticket_id_++,
                        vehicle_id,
                        vehicle_size,
                        space_id,
                        start_time,
                        current_time
                    );

                    fees_collected_ += ticket.getFee();
                    tickets.push_back(ticket);
                }
            }
        }
    }

    // Update mode after freeing spaces
    updateMode();

    return tickets;
}

int Garage::getFeesCollected() const {
    return fees_collected_;
}

int Garage::getCarsParked() const {
    return cars_parked_;
}

GarageStatus Garage::getStatus() const {
    GarageStatus status;
    status.total_spaces = static_cast<int>(spaces_.size());
    status.n1_spaces = n1_spaces_;
    status.n2_spaces = n2_spaces_;
    status.n3_spaces = n3_spaces_;
    status.queue_length = static_cast<int>(queue_.size());
    status.fees_collected = fees_collected_;
    status.cars_parked = cars_parked_;

    // Count occupied spaces by size
    status.occupied_n1 = 0;
    status.occupied_n2 = 0;
    status.occupied_n3 = 0;

    for (const auto& space : spaces_) {
        if (space.isOccupied()) {
            switch (space.getSize()) {
                case Size::SMALL:
                    status.occupied_n1++;
                    break;
                case Size::MEDIUM:
                    status.occupied_n2++;
                    break;
                case Size::LARGE:
                    status.occupied_n3++;
                    break;
            }
        }
    }

    return status;
}

ParkingSpace* Garage::findAvailableSpace(Size vehicle_size) {
    for (auto& space : spaces_) {
        if (!space.isOccupied() && canFit(vehicle_size, space.getSize())) {
            return &space;
        }
    }
    return nullptr;
}

void Garage::setStrategy(std::unique_ptr<IParkingStrategy> strategy) {
    strategy_ = std::move(strategy);
}

ParkingMode Garage::getMode() const {
    return mode_;
}

std::size_t Garage::getQueueSize() const {
    return queue_.size();
}

int Garage::getTotalSpaces() const {
    return static_cast<int>(spaces_.size());
}

void Garage::updateMode() {
    if (queue_.isEmpty()) {
        mode_ = ParkingMode::FIFO;
        return;
    }

    // Check if front vehicle can be parked
    const Vehicle& front = queue_.front();
    bool front_can_park = false;

    for (const auto& space : spaces_) {
        if (space.canAccommodate(front)) {
            front_can_park = true;
            break;
        }
    }

    if (front_can_park) {
        mode_ = ParkingMode::FIFO;
    } else {
        mode_ = ParkingMode::SELECTIVE;
    }
}

}  // namespace pgof
