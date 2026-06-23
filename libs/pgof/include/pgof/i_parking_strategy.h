#pragma once

#include "pgof/vehicle.h"
#include "pgof/parking_space.h"
#include "pgof/wait_queue.h"
#include <vector>

namespace pgof {

/**
 * @brief Interface for parking algorithm selection (Strategy Pattern).
 *
 * Defines the contract for different parking strategies.
 * Implementations decide which vehicle to park and which space to use.
 */
class IParkingStrategy {
public:
    virtual ~IParkingStrategy() = default;

    /**
     * @brief Select which vehicle to park next from the queue.
     * @param queue The waiting queue of vehicles
     * @param spaces Available parking spaces
     * @return Pointer to selected vehicle, or nullptr if none can be parked
     */
    [[nodiscard]] virtual const Vehicle* selectVehicle(
        const WaitQueue& queue,
        const std::vector<ParkingSpace>& spaces) const = 0;

    /**
     * @brief Select which space to use for parking a vehicle.
     * @param vehicle The vehicle to park
     * @param spaces Available parking spaces
     * @return Pointer to selected space, or nullptr if none available
     */
    [[nodiscard]] virtual ParkingSpace* selectSpace(
        const Vehicle& vehicle,
        std::vector<ParkingSpace>& spaces) const = 0;
};

}  // namespace pgof
