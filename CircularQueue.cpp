#include "CircularQueue.hpp"
#include "T2_Ticket.hpp"
#include <stdexcept>

CircularQueue::CircularQueue(int cap)
    : capacity(cap), front(0), rear(-1), currentSize(0) {
    data = new Ticket * [capacity]; // Array of Ticket pointers
}

CircularQueue::~CircularQueue() {
    // Delete all tickets in the queue
    while (!isEmpty()) {
        Ticket* ticket = dequeue();
        delete ticket;
    }
    delete[] data; // Free the array of pointers
}

bool CircularQueue::enqueue(Ticket* ticket) {
    if (isFull()) return false;
    rear = (rear + 1) % capacity;
    data[rear] = ticket;
    currentSize++;
    return true;
}

Ticket* CircularQueue::dequeue() {
    if (isEmpty()) throw std::runtime_error("Queue is empty");
    Ticket* val = data[front];
    front = (front + 1) % capacity;
    currentSize--;
    return val;
}

bool CircularQueue::isFull() const {
    return currentSize == capacity;
}

bool CircularQueue::isEmpty() const {
    return currentSize == 0;
}

int CircularQueue::size() const {
    return currentSize;
}