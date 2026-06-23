#include "pgof/selective_strategy.h"

namespace pgof {

const Vehicle* SelectiveStrategy::selectVehicle(
    const WaitQueue& queue,
    const std::vector<ParkingSpace>& spaces) const {

    // Find first eligible vehicle that can fit in any available space
    return queue.findFirstEligible(spaces);
}

ParkingSpace* SelectiveStrategy::selectSpace(
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
