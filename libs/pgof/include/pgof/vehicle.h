#pragma once

#include "pgof/types.h"

namespace pgof {

/**
 * @brief Represents an autonomous vehicle seeking parking.
 *
 * Each vehicle has a unique ID, a size (1-3), a requested wait time (1-10),
 * and an arrival time for queue ordering.
 */
class Vehicle {
public:
    /**
     * @brief Construct a new Vehicle.
     * @param id Unique vehicle identifier
     * @param size Vehicle size (SMALL, MEDIUM, LARGE)
     * @param requested_wait_time Parking duration in time units (1-10)
     * @param arrival_time Time of arrival for queue ordering
     */
    Vehicle(int id, Size size, int requested_wait_time, int arrival_time);

    // Accessors
    [[nodiscard]] int getId() const;
    [[nodiscard]] Size getSize() const;
    [[nodiscard]] int getRequestedWaitTime() const;
    [[nodiscard]] int getArrivalTime() const;

    /**
     * @brief Check if this vehicle can fit in a space of given size.
     * @param space_size The size of the parking space
     * @return true if vehicle can fit
     */
    [[nodiscard]] bool canFitIn(Size space_size) const;

    // Comparison operators for queue operations
    bool operator==(const Vehicle& other) const;
    bool operator!=(const Vehicle& other) const;

private:
    int id_;
    Size size_;
    int requested_wait_time_;
    int arrival_time_;
};

}  // namespace pgof
