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

    // Getter function for username
    string getUsername() const {
        return username;
    }

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
    vector<User*> users;
    double bankBalance;

public:
    Bank() : bankBalance(0.0) {}

    // Functions for managing users
    void addUser(User* user);
    void removeUser(const string& username);
    User* findUser(const string& username);

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

void Bank::addUser(User* user) {
    users.push_back(user);
}

void Bank::removeUser(const string& username) {
    // Iterate through users and remove the one with the matching username
    for (auto it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUsername() == username) {
            users.erase(it);
            break; // Once found and removed, exit loop
        }
    }
}

User* Bank::findUser(const string& username) {
    // Iterate through users and find the one with the matching usrname
    for (auto& user : users) {
        if (user->getUsername() == username) {
            return user;
        }
    }
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

// function to prompt for admin password
bool promptAdminPassword() {
    string adminPwd;
    cout << "Enter admin password: ";
    cin >> adminPwd;
    return (adminPwd == "adminpassword8");
}

int  createEmployee(Bank& bank) {
    if (!promptAdminPassword()) {
        cout << "Incorrect admin password. Employee creation failed.\n";
        return 0; // Return to the main menu after a failed attempt
    }

    string uname, pwd, name;
    cout << "Enter employee username: ";
    cin >> uname;
    cout << "Enter employee password: ";
    cin >> pwd;
    cout << "Enter employee full name: ";
    cin >> name;
    cin.ignore();

    // Create an Employee object
    Employee* employee = new Employee(uname, pwd, name);

    // Add the Employee object to the bank
    bank.addUser(employee);

    // Store username and password in a text file
    ofstream outFile("employee_credentials.txt", ios::app); // Open file in append mode
    if (outFile.is_open()) {
        // Write username and password to the file
        outFile << uname << " " << pwd << endl;
        outFile.close(); // Close the file
        cout << "Employee created successfully and credentials stored in file.\n";
    } else {
        cout << "Error: Unable to open file for writing.\n";
    }
    return 0;
}

void mainMenu() {
    Bank bank; // Create Bank instance

    int choice1, choice2, choice3;
    do {
        cout << "Welcome to Ventura Bank Management System\n";
        cout << "Do you want to?\n";
        cout << "1. Login\n";
        cout << "2. Sign up\n";
        cout << "3. Exit\n";
        cin >> choice1;

        switch (choice1) {
            case 1:
                cout << "Welcome to Ventura Bank Management System\n";
                cout << "1. Admin Login\n";
                cout << "2. Employee Login\n";
                cout << "3. Customer Login\n";
                cout << "4. Exit\n";
                cout << "Enter your choice: ";
                cin >> choice2;

                switch (choice2) {
                    // Implement login functionality
                }
                break;
            case 2:
                cout << "Select one\n";
                cout << "1. Create employee\n";
                cout << "2. Create customer\n";
                cout << "3. Return to main menu\n";
                cin >> choice3;

                switch (choice3) {
                    case 1:
                        createEmployee(bank);
                        break;
                    case 2:
                        cout << "Customer creation coming soon!\n";
                        break;
                    case 3:
                        break; // Return to main menu
                    default:
                        cout << "Invalid choice.\n";
                }
                break;
            case 3:
                cout << "Exiting program. Goodbye!\n";
                return; // Exit the program
            default:
                cout << "Invalid choice.\n";
        }
    } while (true); // Keep looping until the user chooses to exit
}

int main() {
    mainMenu();
    return 0;
}