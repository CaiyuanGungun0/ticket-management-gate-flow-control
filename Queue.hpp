#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <iostream>
#include "T2_Ticket.hpp"

// Node structure for Queue
struct Node {
    Ticket* data;
    Node* next;
    Node(Ticket* ticket) : data(ticket), next(nullptr) {}
};

class Queue {
private:
    Node* front;
    Node* rear;
    int count;  // Track number of elements

public:
    Queue();
    ~Queue();
    void enqueue(Ticket* ticket);
    Ticket* dequeue();
    bool gen_isEmpty();
    int Qsize() const;
    Ticket* peek();
};

#endif