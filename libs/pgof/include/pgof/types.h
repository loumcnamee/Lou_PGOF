#pragma once

namespace pgof {

/**
 * @brief Size enumeration shared by Vehicle and ParkingSpace.
 *
 * Vehicles and parking spaces come in three sizes.
 * A vehicle can fit in a space if space_size >= vehicle_size.
 */
enum class Size : int {
    SMALL = 1,   ///< Size 1 - smallest
    MEDIUM = 2,  ///< Size 2 - medium
    LARGE = 3    ///< Size 3 - largest
};

/**
 * @brief Parking mode enumeration for garage operation.
 *
 * FIFO: Vehicles are parked in strict arrival order.
 * SELECTIVE: When FIFO is blocked, park the first eligible vehicle.
 */
enum class ParkingMode {
    FIFO,       ///< Park in arrival order (first-in-first-out)
    SELECTIVE   ///< Park first eligible when FIFO is blocked
};

/**
 * @brief Check if a vehicle of given size can fit in a space of given size.
 * @param vehicle_size The size of the vehicle
 * @param space_size The size of the parking space
 * @return true if vehicle can fit in the space
 */
inline bool canFit(Size vehicle_size, Size space_size) {
    return static_cast<int>(space_size) >= static_cast<int>(vehicle_size);
}

}  // namespace pgof
