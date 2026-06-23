#include "pgof/fifo_strategy.h"

namespace pgof {

const Vehicle* FIFOStrategy::selectVehicle(
    const WaitQueue& queue,
    const std::vector<ParkingSpace>& spaces) const {

    if (queue.isEmpty()) {
        return nullptr;
    }

    const Vehicle& front_vehicle = queue.front();

    // Check if the front vehicle can fit in any available space
    for (const auto& space : spaces) {
        if (space.canAccommodate(front_vehicle)) {
            return &front_vehicle;
        }
    }

    // Front vehicle cannot be parked
    return nullptr;
}

ParkingSpace* FIFOStrategy::selectSpace(
    const Vehicle& vehicle,
    std::vector<ParkingSpace>& spaces) const {

    // Find first available space that can accommodate the vehicle
    for (auto& space : spaces) {
        if (space.canAccommodate(vehicle)) {
            return &space;
        }
    }

    return nullptr;
}

}  // namespace pgof
