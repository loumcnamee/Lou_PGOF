#pragma once

#include "pgof/types.h"
#include "pgof/vehicle.h"
#include "pgof/parking_space.h"
#include "pgof/parking_ticket.h"
#include "pgof/garage_status.h"
#include "pgof/wait_queue.h"
#include "pgof/i_parking_strategy.h"
#include <vector>
#include <memory>

namespace pgof {

/**
 * @brief Central coordinator for all parking operations.
 *
 * The Garage class manages parking spaces, the waiting queue,
 * and coordinates vehicle parking/unparking operations.
 * Uses the Strategy pattern for selecting parking algorithms.
 */
class Garage {
public:
    /**
     * @brief Construct a Garage with the specified number of spaces.
     * @param total_spaces Total number of parking spaces (10-1,000,000)
     * @throws std::out_of_range if total_spaces is outside valid range
     */
    explicit Garage(int total_spaces);

    /**
     * @brief Initialize or re-initialize the garage with a new capacity.
     * @param total_spaces Total number of parking spaces
     * @throws std::out_of_range if total_spaces is outside valid range
     */
    void initialize(int total_spaces);

    /**
     * @brief Add a vehicle to the waiting queue.
     * @param vehicle The vehicle to add
     */
    void addVehicle(const Vehicle& vehicle);

    /**
     * @brief Attempt to park the next eligible vehicle.
     * @param current_time Current simulation time
     * @return true if a vehicle was parked
     */
    bool parkNextVehicle(int current_time);

    /**
     * @brief Unpark all vehicles whose wait time has expired.
     * @param current_time Current simulation time
     * @return Vector of parking tickets for unparked vehicles
     */
    std::vector<ParkingTicket> unparkExpiredVehicles(int current_time);

    /**
     * @brief Get the total fees collected.
     * @return Total fees
     */
    [[nodiscard]] int getFeesCollected() const;

    /**
     * @brief Get the total number of cars parked (historical).
     * @return Total cars parked count
     */
    [[nodiscard]] int getCarsParked() const;

    /**
     * @brief Get the current garage status.
     * @return GarageStatus struct with all statistics
     */
    [[nodiscard]] GarageStatus getStatus() const;

    /**
     * @brief Find an available space for a vehicle of given size.
     * @param vehicle_size Size of vehicle to park
     * @return Pointer to available space, or nullptr if none found
     */
    [[nodiscard]] ParkingSpace* findAvailableSpace(Size vehicle_size);

    /**
     * @brief Set the parking strategy to use.
     * @param strategy Unique pointer to strategy implementation
     */
    void setStrategy(std::unique_ptr<IParkingStrategy> strategy);

    /**
     * @brief Get the current parking mode.
     * @return Current ParkingMode (FIFO or SELECTIVE)
     */
    [[nodiscard]] ParkingMode getMode() const;

    /**
     * @brief Get the number of vehicles in the waiting queue.
     * @return Queue size
     */
    [[nodiscard]] std::size_t getQueueSize() const;

    /**
     * @brief Get the total number of spaces.
     * @return Total spaces
     */
    [[nodiscard]] int getTotalSpaces() const;

private:
    std::vector<ParkingSpace> spaces_;
    WaitQueue queue_;
    std::unique_ptr<IParkingStrategy> strategy_;
    int fees_collected_ = 0;
    int cars_parked_ = 0;
    ParkingMode mode_ = ParkingMode::FIFO;
    int next_ticket_id_ = 1;

    // Space distribution counts
    int n1_spaces_ = 0;
    int n2_spaces_ = 0;
    int n3_spaces_ = 0;

    /**
     * @brief Check and update parking mode based on current state.
     */
    void updateMode();

    /**
     * @brief Create parking spaces based on capacity distribution.
     * @param n1 Number of size-1 spaces
     * @param n2 Number of size-2 spaces
     * @param n3 Number of size-3 spaces
     */
    void createSpaces(int n1, int n2, int n3);
};

}  // namespace pgof
