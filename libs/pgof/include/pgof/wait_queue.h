#pragma once

#include "pgof/vehicle.h"
#include "pgof/parking_space.h"
#include <deque>
#include <vector>
#include <optional>

namespace pgof {

/**
 * @brief Manages the FIFO queue of waiting vehicles.
 *
 * Vehicles are added in arrival order and can be removed
 * either from the front (FIFO) or by finding the first
 * eligible vehicle that fits available spaces.
 */
class WaitQueue {
public:
    WaitQueue() = default;

    /**
     * @brief Add a vehicle to the end of the queue.
     * @param vehicle The vehicle to enqueue
     */
    void enqueue(const Vehicle& vehicle);

    /**
     * @brief Remove and return the vehicle at the front of the queue.
     * @return The dequeued vehicle
     * @throws std::runtime_error if queue is empty
     */
    Vehicle dequeue();

    /**
     * @brief Get the vehicle at the front without removing it.
     * @return Reference to the front vehicle
     * @throws std::runtime_error if queue is empty
     */
    [[nodiscard]] const Vehicle& front() const;

    /**
     * @brief Find the first vehicle in queue that can fit in available spaces.
     * @param spaces Vector of parking spaces to check against
     * @return Pointer to the eligible vehicle, or nullptr if none found
     */
    [[nodiscard]] const Vehicle* findFirstEligible(
        const std::vector<ParkingSpace>& spaces) const;

    /**
     * @brief Remove a specific vehicle from the queue.
     * @param vehicle The vehicle to remove
     * @return true if vehicle was found and removed
     */
    bool remove(const Vehicle& vehicle);

    /**
     * @brief Get the number of vehicles in the queue.
     * @return Queue size
     */
    [[nodiscard]] std::size_t size() const;

    /**
     * @brief Check if the queue is empty.
     * @return true if no vehicles in queue
     */
    [[nodiscard]] bool isEmpty() const;

    /**
     * @brief Get iterator access for traversing the queue.
     */
    [[nodiscard]] auto begin() const { return queue_.begin(); }
    [[nodiscard]] auto end() const { return queue_.end(); }

private:
    std::deque<Vehicle> queue_;
};

}  // namespace pgof
