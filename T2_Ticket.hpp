#ifndef TICKET_HPP
#define TICKET_HPP

#include <string>
using namespace std;

struct Ticket {
    string TicketID;
    string TicketType;
    double Price;
    string PurchaseDateTime;
    string BuyerName;
    string BuyerContact;
    string PaymentStatus;
    int Session;
    string EntryGate;
    string TransactionID;
    string ValidityStatus;

    // Constructor
    Ticket() = default;
    Ticket(string id, string type, double price, string datetime, string name,
        string contact, string status, int session, string gate, string txn, string validity)
        : TicketID(id), TicketType(type), Price(price), PurchaseDateTime(datetime),
        BuyerName(name), BuyerContact(contact), PaymentStatus(status),
        Session(session), EntryGate(gate), TransactionID(txn), ValidityStatus(validity) {
    }

    int getPriority() const {
        if (TicketType == "VIP") return 1;
        else if (TicketType == "EarlyBird") return 2;
        else if (TicketType == "General") return 3;
        else return 4; // Default for unknown types
    }

    int getValidity() const {
        if (ValidityStatus == "Valid") return 1;
        else if (ValidityStatus == "Invalid") return 2;
        else return 3; // Default for unknown types
    }
};

#endif 