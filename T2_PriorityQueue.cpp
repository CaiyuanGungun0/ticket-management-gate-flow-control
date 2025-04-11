#include "T2_PriorityQueue.hpp"
#include <iostream>

using namespace std;

PriorityQueue::PriorityQueue() : head(nullptr), count(0) {}

PriorityQueue::~PriorityQueue() {       //Destructor: cleans up both nodes and tickets
    while (head != nullptr) {
        PriorityNode* temp = head;
        head = head->next;
        delete temp->data;  // Delete the ticket
        delete temp;        // Delete the node
    }
}

void PriorityQueue::insertWithPriority(Ticket* ticket, int priority) {
    PriorityNode* newNode = new PriorityNode(ticket, priority);

    if (!head || priority < head->priority) {
        newNode->next = head;
        head = newNode;
        count++;
        return;
    }

    PriorityNode* temp = head;
    while (temp->next && temp->next->priority <= priority) {
        temp = temp->next;
    }
    newNode->next = temp->next;
    temp->next = newNode;
    count++;
}

Ticket* PriorityQueue::removeHighestPriority() {
    if (isEmpty()) {
        cerr << "Priority Queue is empty!" << endl;
        return nullptr;
    }
    PriorityNode* temp = head;
    Ticket* ticket = temp->data;
    head = head->next;
    delete temp;
    count--;
    return ticket;
}

Ticket* PriorityQueue::peek() {
    if (isEmpty()) {
        cerr << "Priority Queue is empty!" << endl;
        return nullptr;
    }
    return head->data; // Return the ticket at the head
}

int PriorityQueue::extractHighestPriority() {
    if (isEmpty()) {
        cerr << "Priority Queue is empty!" << endl;
        return -1;
    }
    PriorityNode* temp = head;
    int priority = temp->priority;
    head = head->next;
    delete temp->data;
    delete temp;
    return priority;
}

bool PriorityQueue::isEmpty() const {
    return head == nullptr;
}

int PriorityQueue::size() const {
    return count;
}
