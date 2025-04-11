#include "Queue.hpp"

Queue::Queue() : front(nullptr), rear(nullptr), count(0) {}

Queue::~Queue() {
    while (!gen_isEmpty()) {
        dequeue();
    }
}

void Queue::enqueue(Ticket* ticket) {
    Node* newNode = new Node(ticket);
    if (gen_isEmpty()) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    count++;
}

Ticket* Queue::dequeue() {
    if (gen_isEmpty()) {
        return nullptr;
    }
    Node* temp = front;
    Ticket* ticket = temp->data;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete temp;
    count--;
    return ticket;
}

bool Queue::gen_isEmpty() {
    return front == nullptr;
}

int Queue::Qsize() const {
    return count;
}

Ticket* Queue::peek() {
    if (gen_isEmpty()) {
        std::cerr << "Queue is empty!" << std::endl;
        return nullptr;
    }
    return front->data;
}