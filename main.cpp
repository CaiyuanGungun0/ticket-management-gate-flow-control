#include <iostream>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <thread> //for loading animation
using namespace std;
#include "T2_TicketManager.hpp"

// Helper functions
void clearScreen() {
    system("cls");
}

void showLoadingAnimation() {
    cout << "\nLoading";
    for (int i = 0; i < 3; i++) {
        cout << ".";
        Sleep(500);
    }
    cout << endl;
}

void setTextColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
    TicketManagement tm;
    tm.loadTicketsFromFile("tickets.csv");
    cout << "tickets.csv file loaded!\n\n" << endl;
    int choice = 0;
    bool running = true;

    while (running) {
        clearScreen();

        cout << "****TICKET SALES AND SPECTATOR MANAGEMENT****" << endl;

        int choice;
        do {
            cout << "\nTicket Sales and Spectator Management System" << endl;
            cout << "1. Sell Ticket" << endl;
            cout << "2. Process Ticket" << endl;
            cout << "3. Display Tickets" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                string id, type, date, name, contact, status, gate, txn, validity;
                double price = 0.00;
                int session = 0;
                /*           Ticket newTicket{ id, type, price, date, name, contact, status, session, gate, txn, validity };*/
                tm.sellTicket();
                cout << endl;
                break;
            }
            case 2:
                int sessionChoice;
                cout << "Enter session to process ticket: ";
                cin >> sessionChoice;
                tm.processSession(sessionChoice);
                /* tm.processTicket();*/
                break;
            case 3:
                tm.displayTickets();
                break;
            case 4:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Try again." << endl;
            }
        } while (choice != 4);

        return 0;
    }
}