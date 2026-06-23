#pragma once

#include "pgof/i_parking_strategy.h"

namespace pgof {

/**
 * @brief FIFO parking strategy - parks vehicles in strict arrival order.
 *
 * Always selects the first vehicle in the queue and finds
 * the first available space that can accommodate it.
 */
class FIFOStrategy : public IParkingStrategy {
public:
    FIFOStrategy() = default;
    ~FIFOStrategy() override = default;

    /**
     * @brief Select the first vehicle in queue (FIFO order).
     * @param queue The waiting queue
     * @param spaces Available parking spaces
     * @return Pointer to front vehicle if it can be parked, nullptr otherwise
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
