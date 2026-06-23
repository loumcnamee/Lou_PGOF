#include "pgof/vehicle.h"

namespace pgof {

Vehicle::Vehicle(int id, Size size, int requested_wait_time, int arrival_time)
    : id_(id)
    , size_(size)
    , requested_wait_time_(requested_wait_time)
    , arrival_time_(arrival_time) {
}

int Vehicle::getId() const {
    return id_;
}

Size Vehicle::getSize() const {
    return size_;
}

int Vehicle::getRequestedWaitTime() const {
    return requested_wait_time_;
}

int Vehicle::getArrivalTime() const {
    return arrival_time_;
}

bool Vehicle::canFitIn(Size space_size) const {
    return canFit(size_, space_size);
}

bool Vehicle::operator==(const Vehicle& other) const {
    return id_ == other.id_;
}

bool Vehicle::operator!=(const Vehicle& other) const {
    return !(*this == other);
}

}  // namespace pgof
