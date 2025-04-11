#include "T2_TicketManager.hpp"
#include <iostream> //for cin and cout
#include <iomanip>  //for setw and setprecision
#include <fstream>  //for ifstream
#include <sstream>  //for stringstream
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>

using namespace std;

void TicketManagement::loadTicketsFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }

    string line;
    // Read first line and check if it's header
    if (getline(file, line)) {
        if (line.substr(0, 8) != "TicketID") {  // If not header, reset file pointer
            file.seekg(0);
        }
    }

    int lineCount = 0;
    while (getline(file, line)) {
        lineCount++;
        //cout << "Processing line " << lineCount << ": " << line << endl;

        stringstream ss(line);
        string id, typeAndPrice, date, name, contact, status, session, gate, txn, validity;

        try {
            if (!getline(ss, id, ',')) throw runtime_error("Failed to read ID");
            if (!getline(ss, typeAndPrice, ',')) throw runtime_error("Failed to read type and price");
            if (!getline(ss, date, ',')) throw runtime_error("Failed to read date");
            if (!getline(ss, name, ',')) throw runtime_error("Failed to read name");
            if (!getline(ss, contact, ',')) throw runtime_error("Failed to read contact");
            if (!getline(ss, status, ',')) throw runtime_error("Failed to read status");
            if (!getline(ss, session, ',')) throw runtime_error("Failed to read status");
            if (!getline(ss, gate, ',')) throw runtime_error("Failed to read gate");
            if (!getline(ss, txn, ',')) throw runtime_error("Failed to read transaction ID");
            if (!getline(ss, validity)) throw runtime_error("Failed to read validity");

            // Parse type and price from format (Type) $Price
            size_t openBracket = typeAndPrice.find('(');
            size_t closeBracket = typeAndPrice.find(')');

            if (openBracket == string::npos || closeBracket == string::npos) {
                throw runtime_error("Invalid type and price format. Expected (Type) $Price");
            }

            string type = typeAndPrice.substr(openBracket + 1, closeBracket - openBracket - 1);
            string priceStr = typeAndPrice.substr(closeBracket + 1);

            // Remove leading/trailing spaces from type
            type.erase(0, type.find_first_not_of(" "));
            type.erase(type.find_last_not_of(" ") + 1);

            // Remove space and $ from price
            priceStr.erase(0, priceStr.find_first_not_of(" "));  // Remove leading spaces
            priceStr.erase(0, priceStr.find_first_not_of("$"));  // Remove $ symbol

            // ***Debug use only***
            //cout << "Parsed values:" << endl;  
            //cout << "ID: " << id << endl;
            //cout << "Type: " << type << endl;
            //cout << "Price string: " << priceStr << endl;

            double ticketPrice;
            try {
                ticketPrice = stod(priceStr);
            }
            catch (const exception& e) {
                throw runtime_error("Failed to convert price: " + priceStr);
            }
            int sessionNum;
            try {
                sessionNum = stoi(session);
            }
            catch (const exception& e) {
                throw runtime_error("Failed to convert session: " + session);
            }
            Ticket newTicket(id, type, ticketPrice, date, name, contact, status, sessionNum, gate, txn, validity);
            addTicket(newTicket);
            //cout << "Successfully added ticket: " << id << endl; ***Debug use only***
        }
        catch (const exception& e) {
            cerr << "Error processing line " << lineCount << ": " << e.what() << endl;
            cerr << "Line content: " << line << endl;
            continue;  // Skipline and continue with next
        }
    }
    cout << "Finished processing " << lineCount << " lines" << endl;
    file.close();
    //To note: csv file constains data below
    //TicketID, (TicketType)Price, PurchaseDateTime, BuyerName, BuyerContact, PaymentStatus, EntryGate, TransactionID, ValidityStatus
}

void TicketManagement::addTicket(const Ticket& ticket) {
    // Create a new Ticket pointer from the passed ticket
    Ticket* newTicket = new Ticket(ticket);

    // Determine priority based on ticket type
    int priority;
    if (newTicket->TicketType == "VIP") {
        priority = 1;
    }
    else if (newTicket->TicketType == "Early Bird") {
        priority = 2;
    }
    else {
        priority = 3;
    }

    // Add to appropriate session queue
    if (newTicket->Session >= 1 && newTicket->Session <= 5) {
        sessionQueues[newTicket->Session - 1].insertWithPriority(newTicket, priority);
    }
    else {
        cerr << "Invalid session number: " << newTicket->Session << endl;
        delete newTicket;
    }
}

void TicketManagement::sellTicket() {
    // Get current date and time
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    tm timeInfo;
    localtime_s(&timeInfo, &currentTime);
    stringstream datetime1;
    datetime1 << put_time(&timeInfo, "%Y-%m-%dT%H:%M:%SZ");
    string datetime = datetime1.str();

    // Get ticket type from user
    int typeChoice;
    string ticketType;
    double price;
    do {
        cout << "Enter ticket type (1=VIP, 2=EarlyBird, 3=General): ";
        if (!(cin >> typeChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue;
        }
        break;
    } while (true);

    switch (typeChoice) {
    case 1:
        ticketType = "VIP";
        price = 150.75;
        break;
    case 2:
        ticketType = "EarlyBird";
        price = 100.50;
        break;
    case 3:
        ticketType = "General";
        price = 50.25;
        break;
    default:
        cout << "Invalid ticket type!" << endl;
        return;
    }
    // Generate ticket ID based on total tickets in all queues
    int totalTickets = 0;
    for (int i = 0; i < 5; i++) {
        totalTickets += sessionQueues[i].size();
    }
    stringstream ss;
    ss << (totalTickets + 1);
    string id = ss.str();

    // Get buyer information
    string name, contact;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    do {
        cout << "Enter buyer name: ";
        getline(cin, name);
        if (name.empty()) {
            cout << "Name cannot be empty. Please try again." << endl;
        }
    } while (name.empty());

    do {
        cout << "Enter buyer contact: ";
        getline(cin, contact);
        if (contact.empty()) {
            cout << "Contact cannot be empty. Please try again." << endl;
        }
    } while (contact.empty());

    // Get payment status with input validation
    int statusChoice;
    string paymentStatus;
    do {
        cout << "Enter payment status (1=Paid, 0=Pending): ";
        if (!(cin >> statusChoice) || (statusChoice != 0 && statusChoice != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 0 or 1." << endl;
            continue;
        }
        break;
    } while (true);
    paymentStatus = (statusChoice == 1) ? "Paid" : "Pending";

    // Get Tournament Session with input validation
    int session;
    do {
        cout << "Enter Session (1=Qualifier, 2=Semi-Final, 3=Final): ";
        if (!(cin >> session) || (session != 1 && session != 2 && session != 3)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 1 or 2." << endl;
            continue;
        }
        break;
    } while (true);

    // Get entry gate with input validation
    string gate;
    do {
        cout << "Enter gate (GateA, GateB, GateC): ";
        cin >> gate;
        if (gate != "GateA" && gate != "GateB" && gate != "GateC") {
            cout << "Invalid gate. Please enter GateA, GateB, or GateC." << endl;
        }
    } while (gate != "GateA" && gate != "GateB" && gate != "GateC");

    // Get validity status with input validation
    int validityChoice;
    string validityStatus;
    do {
        cout << "Enter validity status (1=Valid, 0=Invalid): ";
        if (!(cin >> validityChoice) || (validityChoice != 0 && validityChoice != 1)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 0 or 1." << endl;
            continue;
        }
        break;
    } while (true);
    validityStatus = (validityChoice == 1) ? "Valid" : "Invalid";

    // Generate transaction ID (random 10-digit number)
    stringstream txn1;
    txn1 << rand() % 9000000000 + 1000000000;
    string txn = txn1.str();

    // Stack allocation and passed by reference to addTicket()
    // Create and add the ticket
    try {
        Ticket newTicket(
            id,
            ticketType,
            price,
            datetime,
            name,
            contact,
            paymentStatus,
            session,
            gate,
            txn,
            validityStatus
        );

        addTicket(newTicket);

        cout << "\nTicket successfully added!" << endl;
        cout << "ID: " << id << endl;
        cout << "Type: (" << ticketType << ") $" << fixed << setprecision(2) << price << endl;
        cout << "Gate: " << gate << endl;
        cout << "Session: " << session << endl << endl;
    }
    catch (const exception& e) {
        cout << "Error creating ticket: " << e.what() << endl;
    }
}

void TicketManagement::processTicket() {
    int highestPriority = 4; // Lowest priority (General is 3, default 4)
    int selectedSession = -1;

    // Find the highest-priority ticket across all sessions
    for (int session = 0; session < 5; session++) {
        if (!sessionQueues[session].isEmpty()) {
            Ticket* frontTicket = sessionQueues[session].peek();
            int currPriority = frontTicket->getPriority();
            if (currPriority < highestPriority ||
                (currPriority == highestPriority && selectedSession == -1)) {
                highestPriority = currPriority;
                selectedSession = session;
            }
        }
    }

    // Process the ticket
    if (selectedSession != -1) {
        Ticket* ticket = sessionQueues[selectedSession].removeHighestPriority();
        cout << "Processed ticket ID: " << ticket->TicketID
            << " (Session " << (selectedSession + 1)
            << ", Type: " << ticket->TicketType << ")" << endl;
        // Assign to gate
        spectatorManager.assignToGate(ticket);
        delete ticket;
    }
    else {
        cout << "No tickets to process." << endl;
    }
}

void TicketManagement::displayTickets() {
    cout << "\t\t\tCurrent Tickets Status" << endl;
    cout << "================================================================================" << endl;

    // Prompt user for session input
    int selectedSession;
    do {
        cout << "Enter session to display (1-5): ";
        if (!(cin >> selectedSession) || selectedSession < 1 || selectedSession > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 5." << endl;
        }
        else {
            break;
        }
    } while (true);

    int sessionIndex = selectedSession - 1;

    if (sessionQueues[sessionIndex].isEmpty()) {
        cout << "No tickets in Session " << selectedSession << "." << endl;
        return;
    }

    // Create copies to avoid modifying the original queue
    PriorityQueue tempQueue;
    PriorityQueue tempRestore;

    // Copy tickets to temporary queues
    while (!sessionQueues[sessionIndex].isEmpty()) {
        Ticket* original = sessionQueues[sessionIndex].removeHighestPriority();
        tempRestore.insertWithPriority(original, original->getPriority());
        Ticket* copy = new Ticket(*original);
        tempQueue.insertWithPriority(copy, copy->getPriority());
    }

    // Restore original queue
    while (!tempRestore.isEmpty()) {
        Ticket* original = tempRestore.removeHighestPriority();
        sessionQueues[sessionIndex].insertWithPriority(original, original->getPriority());
    }

    // Display session header
    cout << "\nSession " << selectedSession << " Tickets:" << endl;
    cout << string(80, '-') << endl;
    cout << setw(10) << "Ticket ID"
        << setw(15) << "Type"
        << setw(10) << "Price"
        << setw(25) << "Name"
        << setw(10) << "Gate" << endl;
    cout << string(80, '-') << endl;

    // Display tickets
    while (!tempQueue.isEmpty()) {
        Ticket* ticket = tempQueue.removeHighestPriority();
        cout << setw(10) << ticket->TicketID
            << setw(15) << ticket->TicketType
            << setw(10) << fixed << setprecision(2) << ticket->Price
            << setw(25) << ticket->BuyerName
            << setw(10) << ticket->EntryGate << endl;
        delete ticket; // Delete the copy
    }
    cout << endl;
}

void TicketManagement::processBatch(int session) {
    if (session < 1 || session > 5) {
        cerr << "Invalid session number: " << session << endl;
        return;
    }
    int batchSize = 300; // 3 gates each 100 capacity
    int processed = 0;
    int remainingTickets = sessionQueues[session - 1].size();

    // Process tickets until queue is empty
    while (!sessionQueues[session - 1].isEmpty()) {
        // Display current status
        cout << "\nCurrent Status for Session " << session << ":" << endl;
        spectatorManager.displayGateStatus();
        cout << "Remaining tickets in queue: " << sessionQueues[session - 1].size() << endl;
        cout << "\nPress Enter to process next batch...";
        cin.get();

        // Process one batch
        int batchProcessed = 0;
        PriorityQueue validTickets; // Temporary queue for valid tickets

        // First, collect valid tickets
        while (!sessionQueues[session - 1].isEmpty()) {
            Ticket* ticket = sessionQueues[session - 1].removeHighestPriority();
            if (ticket->ValidityStatus == "Valid") {
                validTickets.insertWithPriority(ticket, ticket->getPriority());
            }
            else {
                delete ticket;
            }
        }

        // Process valid tickets
        while (!validTickets.isEmpty() && batchProcessed < batchSize) {
            Ticket* ticket = validTickets.removeHighestPriority();
            if (ticket) {
                if (spectatorManager.assignToGate(ticket)) {
                    batchProcessed++;
                    processed++;
                    remainingTickets--;
                    ticket->ValidityStatus = "Invalid"; // Mark as used
                }
                else {
                    delete ticket;
                }
            }
        }

        // Put remaining valid tickets back in session queue
        while (!validTickets.isEmpty()) {
            Ticket* ticket = validTickets.removeHighestPriority();
            sessionQueues[session - 1].insertWithPriority(ticket, ticket->getPriority());
        }

        // Display status after processing batch
        cout << "\nStatus after processing batch:" << endl;
        spectatorManager.displayGateStatus();
        cout << "Tickets processed in this batch: " << batchProcessed << endl;
        cout << "Remaining tickets in queue: " << remainingTickets << endl;

        // Wait for spectators to enter venue
        if (batchProcessed > 0) {
            cout << "\nWaiting for spectators to enter venue..." << endl;
            cout << "Press Enter when spectators have entered...";
            cin.get();
            spectatorManager.clearGates(); // Clear gates after spectators enter
        }
    }

    cout << "\nFinished processing Session " << session << endl;
    cout << "Venue Capacity for Session " << session << " now is: "
        << spectatorManager.getVenueOccupancy() << "/"
        << spectatorManager.getMaxVenueCapacity() << endl;

    // Display final gate status
    cout << "\nFinal Gate Status:" << endl;
    spectatorManager.displayGateStatus();
}

void TicketManagement::processSession(int session) {
    std::cout << "\n===== Processing Session " << session << " =====" << std::endl;

    // Show current venue capacity
    cout << "Current Venue Capacity: " << spectatorManager.getVenueOccupancy()
        << "/" << spectatorManager.getMaxVenueCapacity() << endl;

    // Prompt for action
    int choice;
    do {
        cout << "\nChoose action:" << endl;
        cout << "1. Spectator Entry" << endl;
        cout << "2. Spectator Exit" << endl;
        cout << "Enter choice (1-2): ";
        cin >> choice;
    } while (choice != 1 && choice != 2);

    if (choice == 1) {
        // Process spectator entry
        processBatch(session);
    }
    else {
        // Handle spectator exit
        int exitCount;
        do {
            cout << "Enter number of spectators exiting: ";
            cin >> exitCount;
            if (exitCount < 0 || exitCount > spectatorManager.getVenueOccupancy()) {
                cout << "Invalid number. Please enter a number between 0 and "
                    << spectatorManager.getVenueOccupancy() << endl;
            }
        } while (exitCount < 0 || exitCount > spectatorManager.getVenueOccupancy());

        spectatorManager.removeSpectators(exitCount);
        cout << "\nUpdated Venue Status:" << endl;
        spectatorManager.displayGateStatus();
    }
}
