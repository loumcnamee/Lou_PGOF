#pragma once

#include "pgof/types.h"
#include "pgof/vehicle.h"
#include <optional>

namespace pgof {

/**
 * @brief Represents a physical parking spot in the garage.
 *
 * Each space has a size and can hold at most one vehicle.
 * Tracks occupancy state and parking start time for fee calculation.
 */
class ParkingSpace {
public:
    /**
     * @brief Construct a new ParkingSpace.
     * @param id Unique space identifier
     * @param size Space size (SMALL, MEDIUM, LARGE)
     */
    ParkingSpace(int id, Size size);

    // Accessors
    [[nodiscard]] int getId() const;
    [[nodiscard]] Size getSize() const;
    [[nodiscard]] bool isOccupied() const;
    [[nodiscard]] const Vehicle* getParkedVehicle() const;
    [[nodiscard]] int getParkingStartTime() const;

    /**
     * @brief Check if this space can accommodate the given vehicle.
     * @param vehicle The vehicle to check
     * @return true if space is empty and large enough
     */
    [[nodiscard]] bool canAccommodate(const Vehicle& vehicle) const;

    /**
     * @brief Park a vehicle in this space.
     * @param vehicle The vehicle to park
     * @param current_time Current time for tracking parking duration
     * @return true if parking succeeded
     */
    bool park(const Vehicle& vehicle, int current_time);

    /**
     * @brief Remove the parked vehicle from this space.
     * @return The unparked vehicle, or nullopt if space was empty
     */
    std::optional<Vehicle> unpark();

private:
    int id_;
    Size size_;
    bool occupied_ = false;
    std::optional<Vehicle> parked_vehicle_;
    int parking_start_time_ = 0;
};

}  // namespace pgof
