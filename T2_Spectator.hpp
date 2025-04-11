#ifndef SPECTATOR_MANAGER_HPP
#define SPECTATOR_MANAGER_HPP

#include "CircularQueue.hpp"
#include "T2_Ticket.hpp"

class SpectatorManager {
private:
    static const int GATE_CAPACITY = 100;
    CircularQueue gates[3];  // Gate A, B, C
    int venueOccupancy;      // Track total spectators in venue
    int maxVenueCapacity;    // Maximum venue capacity

public:
    SpectatorManager();

    bool assignToGate(Ticket* ticket);
    void displayGateStatus() const;
    void clearAllGates();
    void clearGates();  // Clear gates after spectators enter venue
    void removeSpectators(int count);  // Remove specified number of spectators
    int getVenueOccupancy() const { return venueOccupancy; }
    int getMaxVenueCapacity() const { return maxVenueCapacity; }
    void incrementVenueOccupancy() { venueOccupancy++; }
    void decrementVenueOccupancy() { venueOccupancy--; }
};

#endif