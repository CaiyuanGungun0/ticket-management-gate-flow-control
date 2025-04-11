#ifndef CIRCULAR_QUEUE_HPP
#define CIRCULAR_QUEUE_HPP

// Forward declaration of Ticket class
struct Ticket;  // Changed from 'class' to 'struct' to match T2_Ticket.hpp

class CircularQueue {
private:
    Ticket** data;  // Array of Ticket pointers
    int capacity;   // Maximum capacity (e.g.: 100)
    int front;      // Front index
    int rear;       // Rear index
    int currentSize;    // Track current size

public:
    CircularQueue(int cap);     // Constructor
    ~CircularQueue();           // Destructor
    bool enqueue(Ticket* ticket);
    Ticket* dequeue();
    bool isFull() const;
    bool isEmpty() const;
    int size() const;
};

#endif // CIRCULAR_QUEUE_HPP