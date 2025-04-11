#ifndef T2_PRIORITYQUEUE_HPP
#define T2_PRIORITYQUEUE_HPP
#include <iostream>
#include "T2_Ticket.hpp"
using namespace std;

// Node structure for Priority Queue
struct PriorityNode {
    Ticket* data;
    int priority;
    PriorityNode* next;

    PriorityNode(Ticket* ticket, int p) : data(ticket), priority(p), next(nullptr) {}
};

class PriorityQueue {
private:
    PriorityNode* head;
    int count;  // Track number of elements

public:
    PriorityQueue();
    ~PriorityQueue();
    void insertWithPriority(Ticket* ticket, int priority);  //enqueue
    int extractHighestPriority();
    Ticket* removeHighestPriority();  //dequeue
    bool isEmpty() const;
    int size() const;
    Ticket* peek();
};

#endif