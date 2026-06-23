#pragma once

#include "pgof/i_parking_strategy.h"

namespace pgof {

/**
 * @brief Selective parking strategy - parks first eligible vehicle.
 *
 * When the first vehicle in queue cannot be parked,
 * this strategy finds the first vehicle that can fit
 * in any available space.
 */
class SelectiveStrategy : public IParkingStrategy {
public:
    SelectiveStrategy() = default;
    ~SelectiveStrategy() override = default;

    /**
     * @brief Select the first eligible vehicle from queue.
     * @param queue The waiting queue
     * @param spaces Available parking spaces
     * @return Pointer to first eligible vehicle, or nullptr
     */
    [[nodiscard]] const Vehicle* selectVehicle(
        const WaitQueue& queue,
        const std::vector<ParkingSpace>& spaces) const override;

    /**
     * @brief Select the first available space for the vehicle.
     * @param vehicle The vehicle to park
     * @param spaces Available parking spaces
     * @return Pointer to first compatible space, or nullptr
     */
    [[nodiscard]] ParkingSpace* selectSpace(
        const Vehicle& vehicle,
        std::vector<ParkingSpace>& spaces) const override;
};

}  // namespace pgof
