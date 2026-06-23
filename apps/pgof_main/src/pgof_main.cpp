#include <cstdlib>
#include <iostream>
#include <random>

#include "pgof/garage.h"
#include "pgof/vehicle.h"

using namespace pgof;

// PGOF main application entry point
int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    constexpr int TOTAL_SPACES = 10;
    constexpr int SIMULATION_TIME = 100;
    constexpr int VEHICLES_TO_GENERATE = 25;

    std::cout << "=== PGOF - Parking Garage of the Future ===\n\n";

    // Initialize garage
    Garage garage(TOTAL_SPACES);
    auto status = garage.getStatus();

    std::cout << "Garage initialized with " << status.total_spaces << " spaces:\n";
    std::cout << "  Size-1 (Small):  " << status.n1_spaces << "\n";
    std::cout << "  Size-2 (Medium): " << status.n2_spaces << "\n";
    std::cout << "  Size-3 (Large):  " << status.n3_spaces << "\n\n";

    // Random number generators for simulation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> size_dist(1, 3);
    std::uniform_int_distribution<> wait_dist(1, 10);

    // Generate vehicles at random intervals
    int vehicle_id = 1;
    int vehicles_generated = 0;

    std::cout << "--- Simulation Start ---\n\n";

    for (int time = 0; time < SIMULATION_TIME; ++time) {
        // Generate some vehicles at this time step
        if (vehicles_generated < VEHICLES_TO_GENERATE && time % 2 == 0) {
            Size size = static_cast<Size>(size_dist(gen));
            int wait_time = wait_dist(gen);

            Vehicle vehicle(vehicle_id++, size, wait_time, time);
            garage.addVehicle(vehicle);
            vehicles_generated++;

            std::cout << "[T=" << time << "] Vehicle " << vehicle.getId()
                      << " arrived (size=" << static_cast<int>(size)
                      << ", wait=" << wait_time << ")\n";
        }

        // Try to park vehicles
        while (garage.parkNextVehicle(time)) {
            std::cout << "[T=" << time << "] Vehicle parked. Mode: "
                      << (garage.getMode() == ParkingMode::FIFO ? "FIFO" : "SELECTIVE")
                      << "\n";
        }

        // Unpark expired vehicles
        auto tickets = garage.unparkExpiredVehicles(time);
        for (const auto& ticket : tickets) {
            std::cout << "[T=" << time << "] Vehicle " << ticket.getVehicleId()
                      << " unparked. Fee: $" << ticket.getFee() << "\n";
        }
    }

    // Final status
    std::cout << "\n--- Simulation End ---\n\n";

    status = garage.getStatus();
    std::cout << "=== Final Statistics ===\n";
    std::cout << "Total cars parked:    " << status.cars_parked << "\n";
    std::cout << "Total fees collected: $" << status.fees_collected << "\n";
    std::cout << "Vehicles in queue:    " << status.queue_length << "\n";
    std::cout << "Occupied spaces:      " << status.getTotalOccupied()
              << "/" << status.total_spaces << "\n";

    return 0;
}

