#include <iostream>
#include "auth.h"
#include "bank.h"
#include "util.h"

int main() {
    clear_console();
    std::cout << "Ventura Bank Management System (Refactored)\n";
    std::cout << "1) Customer Login\n2) Customer Register\n3) Exit\n";
    int choice = 0;
    if(!(std::cin >> choice)) {
        std::cerr << "Invalid input. Exiting.\n";
        return 1;
    }
    std::string username;
    Auth auth("data/customer_credentials.txt");
    Bank bank("data/customer_accounts.txt");

    if(choice == 1) {
        std::cout << "Username: ";
        std::cin >> username;
        std::string password;
        std::cout << "Password: ";
        std::cin >> password;
        if(auth.login(username, password)) {
            std::cout << "Login successful.\n";
            // For demo: show bank info
            bank.showAccountsForUser(username);
        } else {
            std::cout << "Login failed.\n";
        }
    } else if(choice == 2) {
        std::cout << "Choose a username: ";
        std::cin >> username;
        std::string password;
        std::cout << "Choose a password: ";
        std::cin >> password;
        if(auth.registerUser(username, password)) {
            std::cout << "Registered successfully. You can now login.\n";
        } else {
            std::cout << "Registration failed (maybe user exists).\n";
        }
    } else {
        std::cout << "Goodbye.\n";
    }

    return 0;
}
