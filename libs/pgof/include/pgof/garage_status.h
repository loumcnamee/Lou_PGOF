#pragma once

namespace pgof {

/**
 * @brief Data transfer object for garage status queries.
 *
 * Contains all statistics and counts for the current garage state.
 */
struct GarageStatus {
    int total_spaces = 0;      ///< Total number of parking spaces
    int n1_spaces = 0;         ///< Number of size-1 spaces
    int n2_spaces = 0;         ///< Number of size-2 spaces
    int n3_spaces = 0;         ///< Number of size-3 spaces
    int occupied_n1 = 0;       ///< Occupied size-1 spaces
    int occupied_n2 = 0;       ///< Occupied size-2 spaces
    int occupied_n3 = 0;       ///< Occupied size-3 spaces
    int queue_length = 0;      ///< Number of vehicles in waiting queue
    int fees_collected = 0;    ///< Total fees collected
    int cars_parked = 0;       ///< Total number of cars parked (historical)

    /**
     * @brief Get total occupied spaces.
     * @return Sum of all occupied spaces
     */
    [[nodiscard]] int getTotalOccupied() const {
        return occupied_n1 + occupied_n2 + occupied_n3;
    }

    /**
     * @brief Get total available spaces.
     * @return Total spaces minus occupied
     */
    [[nodiscard]] int getTotalAvailable() const {
        return total_spaces - getTotalOccupied();
    }
};

}  // namespace pgof
