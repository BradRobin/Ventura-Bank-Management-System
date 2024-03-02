#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Bank; // Forward declaration

// Base class for all users
class User {
protected:
    string username;
    string password;
    string name;

public:
    User(string uname, string pwd, string nm) : username(uname), password(pwd), name(nm) {}

    virtual void menu(Bank& bank) = 0; // Virtual function for user menu
};

// Admin class
class Admin : public User {
public:
    Admin(string uname, string pwd, string nm) : User(uname, pwd, nm) {}

    void menu(Bank& bank) override;
};

// Employee class
class Employee : public User {
public:
    Employee(string uname, string pwd, string nm) : User(uname, pwd, nm) {}

    void menu(Bank& bank) override;
};

// Customer class
class Customer : public User {
private:
    string address;
    string contactInfo;

public:
    Customer(string uname, string pwd, string nm, string addr, string contact) : 
        User(uname, pwd, nm), address(addr), contactInfo(contact) {}

    void menu(Bank& bank) override;
};

// Bank class
class Bank {
private:
    vector<Customer> customers;
    double bankBalance;

public:
    Bank() : bankBalance(0.0) {}

    // Functions for managing customers
    void addCustomer(const Customer& customer);
    void removeCustomer(const string& username);
    Customer* findCustomer(const string& username);

    // Other functions for bank operations
    void deposit(double amount);
    void withdraw(double amount);
    void viewBankInfo() const;
};

void Admin::menu(Bank& bank) {
    // Admin menu
    // Implement admin-specific functionalities here
}

void Employee::menu(Bank& bank) {
    // Employee menu
    // Implement employee-specific functionalities here
}

void Customer::menu(Bank& bank) {
    // Customer menu
    // Implement customer-specific functionalities here
}

void Bank::addCustomer(const Customer& customer) {
    customers.push_back(customer);
}

void Bank::removeCustomer(const string& username) {
    // Remove customer with given username
}

Customer* Bank::findCustomer(const string& username) {
    // Find and return customer with given username
    return nullptr;
}

void Bank::deposit(double amount) {
    // Implement deposit functionality
}

void Bank::withdraw(double amount) {
    // Implement withdrawal functionality
}

void Bank::viewBankInfo() const {
    // Implement function to view bank information
}

void mainMenu(){
    Bank bank; // Create Bank instance

    int choice;
    do {
        cout << "Welcome to Ventura Bank Management System\n";
        cout << "1. Admin Login\n";
        cout << "2. Employee Login\n";
        cout << "3. Customer Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string uname, pwd;
                cout << "Admin Login" << endl;
                cout << "Enter username: ";
                cin >> uname;
                cout << "Enter password: ";
                cin >> pwd;
                if (uname == "ventura" && pwd == "adminpassword8"){
                    Admin admin("ventura", "adminpassword", "Admin");
                    admin.menu(bank);
                } else {
                    cout << "Invalid username or password. Please try again.\n";
                }
                break;
            }
            case 2: {
                // Employee login
                // Implement employee login functionality
                Employee employee("employee", "employee123", "Employee");
                employee.menu(bank);
                break;
            }
            case 3: {
                // Customer login
                // Implement customer login functionality
                Customer customer("customer", "customer123", "Customer", "Address", "Contact");
                customer.menu(bank);
                break;
            }
            case 4:
                cout << "Exiting program. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

}

int main() {
    mainMenu();
    return 0;
}
