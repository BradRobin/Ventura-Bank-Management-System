#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib> // for rand() and srand()
#include <ctime> //for time()
#include <sstream>
#include <iomanip>
#include <conio.h>



using namespace std;

class Bank; // Forward declaration

string generateAccountNumber();
string maskInput(int length);


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
        User(uname, pwd), address(addr), contactInfo(contact), accountBalance(0.0) {
        ifstream inFile("customer_accounts.txt");
        if (!inFile.is_open()){
            cout << "Error: Unable to open customer accounts file.\n";
            return;
        }

        string line;
        while (getline(inFile, line)) {
            // Search for the username in the line
            size_t pos = line.find("Username: " + username);
            if (pos != string::npos) {
                // Extract account number and balance from the line
                size_t accNumPos = line.find("Account Number: ");
                size_t balancePos = line.find("Initial Balance: ");
                if (accNumPos != string::npos && balancePos != string::npos) {
                    string accNumStr = line.substr(accNumPos + 16, balancePos - (accNumPos + 16));
                    accountNumber = accNumStr;
                    string balanceStr = line.substr(balancePos + 16, line.find(" ksh", balancePos) - (balancePos + 16));
                    accountBalance = stod(balanceStr);
                    inFile.close();
                    return;  // Account found and initialized
                }
            }
        }

        inFile.close();

        // Account not found
        cout << "Customer account not found.\n";
    }

    void menu(Bank& bank) override;

    double getAccountBalance() const {
        return accountBalance;
    }

    void withdraw(double amount){
        if (amount > accountBalance){
            cout << "Insufficient funds." <<endl;
        } else {
            accountBalance -= amount;
            cout << "Withdrawal of " << amount << " Ksh successful!" <<endl;
            cout << "Remaining balance: " << accountBalance << " Ksh" <<endl;
        }
    }

    void deposit(double amount){
        accountBalance += amount;
        cout << "Deposit of " << amount << " Ksh successful!" <<endl;
        cout << "Remaining balance: " << accountBalance << " Ksh" <<endl;
    }

    void createCustomerAccount() {
        cout << "Enter your National ID number: ";
        cin >> nationalID;
        cout << "Enter a 4-digit PIN for your account: ";
        pin = maskInput(4);
        cout << "Enter the initial deposit amount (minimum 500 Ksh): ";
        cin >> accountBalance;

        if (accountBalance < 500){
            cout << "Initial deposit amount must be at least 500 Ksh." << endl;
            return;
        }

        accountNumber = generateAccountNumber();

        cout << "Your account has been created successfully." << endl;
        cout << "Account number: " << accountNumber << endl;
        cout << "Initial balance: " << accountBalance << " Ksh" << endl;

        ofstream outFile("customer_accounts.txt", ios::app);
        if (outFile.is_open()){
            outFile << "Username: " << getUsername() << ", ";
            outFile << "Account Number: " << accountNumber << ", ";
            outFile << "Initial Balance: " << accountBalance << " Ksh" << endl;
            outFile.close();
        } else {
            cout << "Error: Unable to open file for writing." << endl;
        }
    }

    string getUsername() const {
        return username;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    string getPin() const {
        return pin;
    }

    bool authenticatePassword(const string& passwordAttempt) const {
        return passwordAttempt == password;
    }
};

string maskInput(int length) {
    string input;
    char ch;
    while (true) {
        ch = _getch(); // reads a character from the keyboard without displaying it
        if (ch == '\r' || ch == '\n') {
            cout << endl;
            break; // Break loop if Enter key is pressed
        }
        else if (ch == '\b' && !input.empty()) {
            // If Backspace is pressed, erase last character from both string and display
            cout << "\b \b"; // Move cursor back, erase character, move cursor back again
            input.pop_back();
        }
        else if (ch != '\b') { // Ignore Backspace character
            input += ch;
            cout << "*"; // displays an asterisk for privacy
        }
    }
    return input;
}

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
    void deposit(const string& accountNumber, double amount);
    void withdraw(const string& accountNumber, double amount);
    void viewBankInfo() const;
    void updateCustomerAccount(const Customer& customer);
};