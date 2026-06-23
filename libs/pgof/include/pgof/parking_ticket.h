#pragma once

#include "pgof/types.h"

namespace pgof {

/**
 * @brief Records a parking transaction for fee calculation.
 *
 * Created when a vehicle is unparked, containing all information
 * needed to calculate and record the parking fee.
 */
class ParkingTicket {
public:
    /**
     * @brief Construct a new ParkingTicket.
     * @param ticket_id Unique ticket identifier
     * @param vehicle_id ID of the parked vehicle
     * @param vehicle_size Size of the parked vehicle
     * @param space_id ID of the parking space used
     * @param start_time Time when vehicle was parked
     * @param end_time Time when vehicle was unparked
     */
    ParkingTicket(int ticket_id, int vehicle_id, Size vehicle_size,
                  int space_id, int start_time, int end_time);

    // Accessors
    [[nodiscard]] int getTicketId() const;
    [[nodiscard]] int getVehicleId() const;
    [[nodiscard]] int getSpaceId() const;
    [[nodiscard]] int getStartTime() const;
    [[nodiscard]] int getEndTime() const;

    /**
     * @brief Calculate the parking fee.
     * @return Fee = vehicle_size * parking_time
     */
    [[nodiscard]] int calculateFee() const;

    /**
     * @brief Get the pre-calculated fee.
     * @return The parking fee
     */
    [[nodiscard]] int getFee() const;

private:
    int ticket_id_;
    int vehicle_id_;
    Size vehicle_size_;
    int space_id_;
    int start_time_;
    int end_time_;
    int fee_;
};

}  // namespace pgof
