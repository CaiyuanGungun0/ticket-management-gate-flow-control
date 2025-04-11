#ifndef TICKET_MANAGEMENT_HPP
#define TICKET_MANAGEMENT_HPP

#include <iostream>
#include <fstream> 
#include <sstream>
#include "Queue.hpp"
#include "T2_Ticket.hpp"
#include "T2_Spectator.hpp"
#include "T2_PriorityQueue.hpp"

using namespace std;

class TicketManagement {
private:
    PriorityQueue sessionQueues[5];
    SpectatorManager spectatorManager;
    //Queue generalQueue;
    //PriorityQueue vipQueue;
    //PriorityQueue earlyBirdQueue;

public:
    void loadTicketsFromFile(const string& filename);
    void addTicket(const Ticket& ticket);
    void sellTicket();
    void processTicket();
    void displayTickets();
    //void displayTicketbySession();
    void processBatch(int session);
    void processSession(int session);
};

#endif