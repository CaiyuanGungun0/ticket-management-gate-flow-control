#include "T2_Spectator.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

SpectatorManager::SpectatorManager()
    : venueOccupancy(0), maxVenueCapacity(1000),
    gates{ CircularQueue(GATE_CAPACITY), CircularQueue(GATE_CAPACITY), CircularQueue(GATE_CAPACITY) } {
}

bool SpectatorManager::assignToGate(Ticket* ticket) {
    // Find the least full gate
    int leastFullGate = 0;
    int minOccupancy = gates[0].size();

    for (int i = 1; i < 3; i++) {
        if (gates[i].size() < minOccupancy) {
            minOccupancy = gates[i].size();
            leastFullGate = i;
        }
    }

    // Check if venue is at capacity
    if (venueOccupancy >= maxVenueCapacity) {
        std::cout << "Venue is at maximum capacity!" << std::endl;
        return false;
    }

    // Assign to the least full gate
    if (gates[leastFullGate].enqueue(ticket)) {
        incrementVenueOccupancy();
        return true;
    }
    return false;
}

void SpectatorManager::displayGateStatus() const {
    std::cout << "Gate Status:" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::setw(10) << "Gate" << std::setw(20) << "Occupancy" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (int i = 0; i < 3; i++) {
        char gateName = 'A' + i;
        std::cout << std::setw(10) << "Gate " << gateName
            << std::setw(20) << gates[i].size() << "/" << GATE_CAPACITY << std::endl;
    }

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "Venue: " << venueOccupancy << "/" << maxVenueCapacity << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
}

void SpectatorManager::clearAllGates() {
    for (int i = 0; i < 3; i++) {
        while (!gates[i].isEmpty()) {
            Ticket* ticket = gates[i].dequeue();
            delete ticket;
            decrementVenueOccupancy();
        }
    }
}

void SpectatorManager::clearGates() {
    // Clear all gates but maintain venue occupancy
    for (int i = 0; i < 3; i++) {
        while (!gates[i].isEmpty()) {
            Ticket* ticket = gates[i].dequeue();
            delete ticket;
        }
    }
}

void SpectatorManager::removeSpectators(int count) {
    if (count > venueOccupancy) {
        count = venueOccupancy;  // Don't remove more than we have
    }
    venueOccupancy -= count;
}