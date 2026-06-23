#include "pgof/parking_space.h"

namespace pgof {

ParkingSpace::ParkingSpace(int id, Size size)
    : id_(id)
    , size_(size) {
}

int ParkingSpace::getId() const {
    return id_;
}

Size ParkingSpace::getSize() const {
    return size_;
}

bool ParkingSpace::isOccupied() const {
    return occupied_;
}

const Vehicle* ParkingSpace::getParkedVehicle() const {
    if (parked_vehicle_.has_value()) {
        return &parked_vehicle_.value();
    }
    return nullptr;
}

int ParkingSpace::getParkingStartTime() const {
    return parking_start_time_;
}

bool ParkingSpace::canAccommodate(const Vehicle& vehicle) const {
    return !occupied_ && vehicle.canFitIn(size_);
}

bool ParkingSpace::park(const Vehicle& vehicle, int current_time) {
    if (occupied_ || !vehicle.canFitIn(size_)) {
        return false;
    }

    parked_vehicle_ = vehicle;
    occupied_ = true;
    parking_start_time_ = current_time;
    return true;
}

std::optional<Vehicle> ParkingSpace::unpark() {
    if (!occupied_) {
        return std::nullopt;
    }

    std::optional<Vehicle> vehicle = std::move(parked_vehicle_);
    parked_vehicle_.reset();
    occupied_ = false;
    parking_start_time_ = 0;
    return vehicle;
}

}  // namespace pgof
