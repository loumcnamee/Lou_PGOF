#include "pgof/wait_queue.h"
#include <stdexcept>
#include <algorithm>

namespace pgof {

void WaitQueue::enqueue(const Vehicle& vehicle) {
    queue_.push_back(vehicle);
}

Vehicle WaitQueue::dequeue() {
    if (queue_.empty()) {
        throw std::runtime_error("Cannot dequeue from empty queue");
    }

    Vehicle vehicle = queue_.front();
    queue_.pop_front();
    return vehicle;
}

const Vehicle& WaitQueue::front() const {
    if (queue_.empty()) {
        throw std::runtime_error("Cannot access front of empty queue");
    }
    return queue_.front();
}

const Vehicle* WaitQueue::findFirstEligible(
    const std::vector<ParkingSpace>& spaces) const {

    for (const auto& vehicle : queue_) {
        // Check if any available space can accommodate this vehicle
        for (const auto& space : spaces) {
            if (space.canAccommodate(vehicle)) {
                return &vehicle;
            }
        }
    }
    return nullptr;
}

bool WaitQueue::remove(const Vehicle& vehicle) {
    auto it = std::find(queue_.begin(), queue_.end(), vehicle);
    if (it != queue_.end()) {
        queue_.erase(it);
        return true;
    }
    return false;
}

std::size_t WaitQueue::size() const {
    return queue_.size();
}

bool WaitQueue::isEmpty() const {
    return queue_.empty();
}

}  // namespace pgof
