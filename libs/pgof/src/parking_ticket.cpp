#include "pgof/parking_ticket.h"

namespace pgof {

ParkingTicket::ParkingTicket(int ticket_id, int vehicle_id, Size vehicle_size,
                             int space_id, int start_time, int end_time)
    : ticket_id_(ticket_id)
    , vehicle_id_(vehicle_id)
    , vehicle_size_(vehicle_size)
    , space_id_(space_id)
    , start_time_(start_time)
    , end_time_(end_time)
    , fee_(calculateFee()) {
}

int ParkingTicket::getTicketId() const {
    return ticket_id_;
}

int ParkingTicket::getVehicleId() const {
    return vehicle_id_;
}

int ParkingTicket::getSpaceId() const {
    return space_id_;
}

int ParkingTicket::getStartTime() const {
    return start_time_;
}

int ParkingTicket::getEndTime() const {
    return end_time_;
}

int ParkingTicket::calculateFee() const {
    int parking_time = end_time_ - start_time_;
    int size_value = static_cast<int>(vehicle_size_);
    return size_value * parking_time;
}

int ParkingTicket::getFee() const {
    return fee_;
}

}  // namespace pgof
