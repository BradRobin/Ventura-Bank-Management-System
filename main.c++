#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> // for rand() and srand()
#include <ctime> //for time()

using namespace std;

class Bank; // Forward declaration

// Base class for all users
class User {
protected:
    string username;
    string password;

public:
    User(string uname, string pwd) : username(uname), password(pwd) {}

    // Getter function for username
    string getUsername() const {
        return username;
    }

    virtual void menu(Bank& bank) = 0; // Virtual function for user menu
};

// Admin class
class Admin : public User {
public:
    Admin(string uname, string pwd) : User(uname, pwd) {}

    void menu(Bank& bank) override;
};

// Employee class
class Employee : public User {
public:
    Employee(string uname, string pwd) : User(uname, pwd) {}

    void menu(Bank& bank) override;
};

// Customer class
class Customer : public User {
private:
    string address;
    string contactInfo;
    string nationalID;
    string pin;
    double accountBalance;
    string accountNumber;

public:
    Customer(string uname, string pwd, string addr, string contact) : 
        User(uname, pwd), address(addr), contactInfo(contact), accountBalance(0.0) {}

    void menu(Bank& bank) override;

    void createCustomerAccount();
    //Getter function for username
    string getUsername() const {
        return username;
    }
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
    std::cout << "Enter admin password: ";
    std::cin >> adminPwd;
    return (adminPwd == "adminpassword8");
}

string generateAccountNumber(){
    srand(time(0)); //seed for random number generator
    string accountNumber = "";
    for (int i = 0; i < 12; ++i){
        accountNumber += to_string(rand() % 10); // geberate a random digit and append it to the account number
    }

    return accountNumber;
}

int  createEmployee(Bank& bank) {
    if (!promptAdminPassword()) {
        std::cout << "Incorrect admin password. Employee creation failed.\n";
        return 0; // Return to the main menu after a failed attempt
    }

    string uname, pwd, name;
    std::cout << "Enter employee username: ";
    std::cin >> uname;
    std::cout << "Enter employee password: ";
    std::cin >> pwd;
    std::cout << "Enter employee full name: ";
    std::cin >> name;
    std::cin.ignore();

    // Create an Employee object
    Employee* employee = new Employee(uname, pwd);

    // Add the Employee object to the bank
    bank.addUser(employee);

    // Store username and password in a text file
    ofstream outFile("employee_credentials.txt", ios::app); // Open file in append mode
    if (outFile.is_open()) {
        // Write username and password to the file
        outFile << uname << " " << pwd << endl;
        outFile.close(); // Close the file
        std::cout << "Employee created successfully and credentials stored in file.\n";
    } else {
        std::cout << "Error: Unable to open file for writing.\n";
    }
    return 0;
}

bool customerLogin(const string& customerUsername, const string& customerPassword){
    ifstream inFile("customer_credentials.txt");
    if (!inFile.is_open()){
        std::cout << "Error: Unable to open customer credentials file.\n";
        return false;
    }
    string fileUsername, filePassword;
    while (inFile >> fileUsername >> filePassword){
        if (fileUsername == customerUsername && filePassword == customerPassword){
            inFile.close();
            return true; // customerUsername and customerPassword match
        }
    }
    inFile.close();
    return false; // No matching username and password found
}

bool employeeLogin(const string& employeeUsername, const string& employeePassword){
    ifstream inFile("employee_credentials.txt");
    if (!inFile.is_open()){
        cout << "Error: Unable to open employee credentials file." << endl;
        return false;
    }
    string fileUsername, filePassword;
    while (inFile >> fileUsername >> filePassword){
        if (fileUsername == employeeUsername && filePassword == employeePassword){
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

void customerMenu(Customer& customer){
    int choice;
    cout << "Welcome, " << customer.getUsername() << "!" << endl;
    cout << "1. Create a bank account" << endl;
    cout << "2. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            customer.createCustomerAccount();
            break;
        case 2:
            cout << "Existing customer menu." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}

void employeeMenu(const string& employeeUsername){
    std::cout << "Welcome, " << employeeUsername << endl;
}

void Customer::createCustomerAccount(){
    cout << "Enter your National ID number: ";
        cin >> nationalID;
        cout << "Enter a 4-digit PIN for you account: ";
        cin >> pin;
        cout << "Enter the inital deposit amount (minimum 500 ksh): ";
        cin >> accountBalance;

        if (accountBalance < 500){
            cout << "Initial deposit amount must be at least 500ksh." << endl;
            return;
        }

        accountNumber = generateAccountNumber();

        cout << "Your account has been created successfully." << endl;
        cout << "Account number: " << accountNumber << endl;
        cout << "Initial balance: " << accountBalance << " ksh" << endl;

        ofstream outFile("customer_accounts.txt", ios::app);
        if (outFile.is_open()){
            outFile << "Username: " << username << ", ";
            outFile << "Account Number: " << accountNumber << ", ";
            outFile << "Initial Balance: " << accountBalance << " ksh" << endl;
            outFile.close();
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
}

void Customer::menu(Bank& bank){
    customerMenu(*this);
}

void mainMenu() {
    Bank bank; // Create Bank instance

    int choice1, choice2, choice3;
    string adminPasscode;
    string adminName;
    string customerUsername;
    string customerPassword;
    string employeeUsername;
    string employeePassword;


    do {
        std::cout << "Welcome to Ventura Bank Management System\n";
        std::cout << "Do you want to?\n";
        std::cout << "1. Login\n";
        std::cout << "2. Sign up\n";
        std::cout << "3. Exit\n";
        std::cin >> choice1;

        switch (choice1) {
            case 1:
                std::cout << "Welcome to Ventura Bank Management System\n";
                std::cout << "1. Admin Login\n";
                std::cout << "2. Employee Login\n";
                std::cout << "3. Customer Login\n";
                std::cout << "4. Exit\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice2;

                switch (choice2) {
                    case 1:
                        std::cout << "Enter admin username: ";
                        std::cin >> adminName;
                        std::cout << "Enter admin password: ";
                        std::cin >> adminPasscode;

                        if (adminName == "ventura" && adminPasscode == "adminpassword8") {
                            std::cout << "Login Successful";
                        } else {
                            std::cout << "Login Unsuccessful. Try again!";
                            mainMenu();
                        }
                        break;

                    case 2:
                        std::cout << "Enter your username: ";
                        std::cin >> employeeUsername;
                        std::cout << "Enter your username: ";
                        std::cin >> employeePassword;

                        if (employeeLogin(employeeUsername, employeePassword)){
                            std::cout << "Login successful!" << endl;
                            employeeMenu(employeeUsername);
                        } else {
                            std::cout << "Invalid username or password. Please try again" << endl;
                        }
                        break;

                    case 3:
                        std::cout << "Enter your username: ";
                        std::cin >> customerUsername;
                        std::cout << "Enter your password: ";
                        std::cin >> customerPassword;

                        if (customerLogin(customerUsername, customerPassword)){
                            std::cout << "Login successful!" << endl;
                            Customer customer(customerUsername, customerPassword, "", "");
                            customerMenu(customer);
                        } else {
                            std::cout << "Invalid username or password." << endl;
                        }
                        break;
                }
                break;
            case 2:
                std::cout << "Select one\n";
                std::cout << "1. Create employee\n";
                std::cout << "2. Create customer\n";
                std::cout << "3. Return to main menu\n";
                std::cin >> choice3;

                switch (choice3) {
                    case 1:
                        createEmployee(bank);
                        break;
                    case 2:
                        createCustomer(bank);
                        break;
                    case 3:
                        break; // Return to main menu
                    default:
                        std::cout << "Invalid choice.\n";
                }
                break;
            case 3:
                std::cout << "Exiting program. Goodbye!\n";
                return; // Exit the program
            default:
                std::cout << "Invalid choice.\n";
        }
    } while (true); // Keep looping until the user chooses to exit
}

int main() {
    mainMenu();
    return 0;
}