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

    //virtual destructor
    virtual ~User() {}
};

// Admin class
class Admin : public User {
public:
    Admin(string uname, string pwd) : User(uname, pwd) {}

    void menu(Bank& bank) override {
        int choice;
        do {
            cout << "\nAdmin Menu" << endl;
            cout << "1. View Employees" << endl;
            cout << "2. View Customers" << endl;
            cout << "3. Exit" <<endl;
            cout << "Enter your choice: ";
            cin >> choice;
            system("cls");

            switch (choice) {
                case 1:
                    viewEmployees();
                    break;
                case 2:
                    viewCustomers();
                    break;
                case 3:
                    cout << "Exiting Admin Menu..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please enter again." << endl;
            }
        } while (choice != 3);
    }

    void viewEmployees() {
        cout << "\nEmployees:\n";
        ifstream inFile("employee_credentials.txt");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                cout << line << endl;
            }
            inFile.close();
        } else {
            cout << "Unable to open employee credentials file." << endl;
        }
    }

    void viewCustomers() {
        cout << "\nCustomers\n";
        ifstream inFile("customer_accounts.txt");
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                cout << line << endl;
            }
            inFile.close();
        } else {
            cout << "Unable to open customer accounts file." << endl;
        }
    }
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
        system("cls");
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
        system("cls");

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

//void Admin::menu(Bank& bank) {
    // Admin menu
    // Implement admin-specific functionalities here
//}

void Employee::menu(Bank& bank) {
    // Employee menu
    // Implement employee-specific functionalities here
}

void Bank::addUser(User* user) {
    users.push_back(user);
}

void Bank::removeUser(const string& username) {
    // Iterate through users and remove the one with the matching username
    for (vector<User*>::iterator it = users.begin(); it != users.end(); ++it) {
        if ((*it)->getUsername() == username) {
            users.erase(it);
            break; // Once found and removed, exit loop
        }
    }
}


void Bank::withdraw(const string& accountNumber, double amount) {
    ifstream inFile("customer_accounts.txt");
    ofstream outFile("temp_customer_accounts.txt");

    if (!inFile.is_open() || !outFile.is_open()){
        std::cout << "Error: Unable to open customer accounts file.\n";
        return;
    }

    string fileAccountNumber, filePassword, fileUsername;
    double fileBalance;

    bool found = false;

    while (inFile >> fileAccountNumber >> fileUsername >> filePassword >> fileBalance){
        if (fileAccountNumber == accountNumber){
            found = true;
            if (amount <= fileBalance){
                fileBalance -= amount;
            } else {
                cout << "Insufficient funds." << endl;
                inFile.close();
                outFile.close();
                return;
            }
        } else {
            // Write the unchanged account information to the temp file
            outFile << fileAccountNumber << " " << fileUsername << " " << filePassword << " " << fileBalance << endl;
        }
    }

    inFile.close();
    outFile.close();

    if (!found){
        cout << "Customer account not found." << endl;
    } else {
        // Remove the old file
        remove("customer_accounts.txt");

        // Rename temp file to original file
        rename("temp_customer_accounts.txt", "customer_accounts.txt");

        cout << "Withdrawal successful!" << endl;
    }
}


void Bank::deposit(const string& accountNumber, double amount) {
    ifstream inFile("customer_accounts.txt");
    ofstream outFile("temp_customer_accounts.txt");

    if (!inFile.is_open() || !outFile.is_open()){
        cout << "Error: Unable to open customer accounts file.\n";
        return;
    }

    string fileAccountNumber, filePassword, fileUsername;
    double fileBalance;

    bool found = false;

    while (inFile >> fileAccountNumber >> fileUsername >> filePassword >> fileBalance){
        if (fileAccountNumber == accountNumber){
            found = true;
            fileBalance += amount;
            std::cout << "Insufficient funds." << endl;
            inFile.close();
            outFile.close();
            return;
        } else {
            // Write the unchanged account information to the temp file
            outFile << fileAccountNumber << " " << fileUsername << " " << filePassword << " " << fileBalance << endl;
        }
    }

    inFile.close();
    outFile.close();

    if (!found){
        std::cout << "Customer account not found." << endl;
    } else {
        // Remove the old file
        remove("customer_accounts.txt");

        // Rename temp file to original file
        rename("temp_customer_accounts.txt", "customer_accounts.txt");

        cout << "Withdrawal successful!" << endl;
    }
}


void Bank::viewBankInfo() const {
    // Implement function to view bank information
}

// function to prompt for admin password
bool promptAdminPassword() {
    std::cout << "Enter admin password: ";
    string adminPwd = maskInput(20);
    return (adminPwd == "adminpassword8");
}

string generateAccountNumber(){
    srand(time(0)); //seed for random number generator
    stringstream ss;
    for (int i = 0; i < 12; ++i){
        ss << rand() % 10; // geberate a random digit and append it to the account number
    }

    return ss.str();
}

void withdrawFunds(Customer& customer) {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    // Validate username and password
    if (customer.getUsername() == username && customer.authenticatePassword(password)) {
        cout << "Authentication successful!" << endl;
        
        // Display account number and balance
        cout << "Account Number: " << customer.getAccountNumber() << endl;
        cout << "Current Balance: " << customer.getAccountBalance() << " Ksh" << endl;

        double withdrawalAmount;
        cout << "Enter the amount you want to withdraw: ";
        cin >> withdrawalAmount;

        // Validate withdrawal amount
        if (withdrawalAmount > 0 && withdrawalAmount <= customer.getAccountBalance()) {
            string pin;
            cout << "Enter your account PIN: ";
            cin >> pin;

            // Validate PIN
            if (pin == customer.getPin()) {
                // Process withdrawal
                customer.withdraw(withdrawalAmount);
                cout << "Withdrawal successful!" << endl;
            } else {
                cout << "Incorrect PIN. Withdrawal failed." << endl;
            }
        } else {
            cout << "Invalid withdrawal amount. Withdrawal failed." << endl;
        }
    } else {
        cout << "Invalid username or password. Authentication failed." << endl;
    }
}


void depositFunds(Customer& customer) {
    string username;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    string password = maskInput(20);

    // Validate username and password
    if (customer.getUsername() == username && customer.authenticatePassword(password)) {
        cout << "Authentication successful!" << endl;
        
        // Display account number and balance
        cout << "Account Number: " << customer.getAccountNumber() << endl;
        cout << "Current Balance: " << customer.getAccountBalance() << " Ksh" << endl;

        double depositAmount;
        cout << "Enter the amount you want to deposit: ";
        cin >> depositAmount;
        system("cls");

        // Validate withdrawal amount
        if (depositAmount > 0 && depositAmount <= customer.getAccountBalance()) {
            string pin;
            cout << "Enter your account PIN: ";
            cin >> pin;

            // Validate PIN
            if (pin == customer.getPin()) {
                // Process withdrawal
                customer.deposit(depositAmount);
                cout << "deposit successful!" << endl;
            } else {
                cout << "Incorrect PIN. deposit failed." << endl;
            }
        } else {
            cout << "Invalid deposit amount. deposit failed." << endl;
        }
    } else {
        cout << "Invalid username or password. Authentication failed." << endl;
    }
}

void currencyExchange() {
    int option;
    int option2;
    double amount;
    float usd_to_ksh_rate = 139.989;
    float yen_to_ksh_rate = 0.903;
    float euro_to_ksh_rate = 145.773;
    float pound_to_ksh_rate = 170.647;
    
    std::cout << "Currency exchange Menu" << endl;
    std::cout << "1. US Dollar to Ksh" << endl;
    std::cout << "2. Ksh to US Dollar" << endl;
    std::cout << "3. Yen to Ksh" << endl;
    std::cout << "4. Ksh to Yen" << endl;
    std::cout << "5. Euro to Ksh" << endl;
    std::cout << "6. Ksh to Euro" << endl;
    std::cout << "7. Pound to Ksh" << endl;
    std::cout << "8. Ksh to Pound" << endl;
    std::cout << "Enter your choice: ";
    cin >> option;

    std::cout << "Enter the amount: ";
    cin >> amount;

    switch (option){
        case 1:
            std::cout << amount << " USD = " << amount * usd_to_ksh_rate << " Ksh" << endl;
            break;
        case 2:
            std::cout << amount << " Ksh = " << amount / usd_to_ksh_rate << " USD" << endl;
            break;
        case 3:
            std::cout << amount << " Yen = " << amount * yen_to_ksh_rate << " Ksh" << endl;
            break;
        case 4:
            cout << amount << " Ksh = " << amount / yen_to_ksh_rate << " Yen" << endl;
            break;
        case 5:
            cout << amount << " Euro = " << amount * euro_to_ksh_rate << " Ksh" << endl;
            break;
        case 6:
            cout << amount << " Ksh = " << amount / euro_to_ksh_rate << " Euro" << endl;
            break;
        case 7:
            cout << amount << " Pound = " << amount * pound_to_ksh_rate << " Ksh" << endl;
            break;
        case 8:
            cout << amount << " Ksh = " << amount / pound_to_ksh_rate << " Pound" << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}


int createEmployee(Bank& bank) {
    if (!promptAdminPassword()) {
        std::cout << "Incorrect admin password. Employee creation failed.\n";
        return 0; // Return to the main menu after a failed attempt
    }

    string uname, pwd, name;
    std::cout << "Enter employee username: ";
    std::cin >> uname;
    std::cout << "Enter employee password: ";
    pwd = maskInput(20);
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

void createCustomer(Bank& bank) {
    string uname, pwd, addr, contact;
    cout << "Enter customer username: ";
    cin >> uname;
    cout << "Enter customer password: ";
    cin >> pwd;
    cout << "Enter customer address: ";
    cin >> addr;
    cout << "Enter customer contact information: ";
    cin >> contact;

    // Create a Customer object
    Customer* customer = new Customer(uname, pwd, addr, contact);

    // Add the Customer object to the bank
    bank.addUser(customer);

    // Store username and password in a text file
    ofstream outFile("customer_credentials.txt", ios::app); // Open file in append mode
    if (outFile.is_open()) {
        // Write username and password to the file
        outFile << uname << " " << pwd << endl;
        outFile.close(); // Close the file
        cout << "Customer created successfully and credentials stored in file.\n";
    } else {
        cout << "Error: Unable to open file for writing.\n";
    }
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
    cout << "2. Check bank balance" << endl;
    cout << "3. Withdraw funds" << endl;
    cout << "4. Deposit funds" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    system("cls");

    switch (choice) {
        case 1:
            customer.createCustomerAccount();
            break;
        case 2:
            system("cls");
            cout << "Your bank balance is: " << customer.getAccountBalance() << endl;
            break;
        case 3: {
            system("cls");
            cout << "Enter your account password: ";
            string password = maskInput(20);

            if (!customer.authenticatePassword(password)){
                cout << "Invalid password. Access denied." << endl;
                break;
            }

            cout << "Your account number: " << customer.getAccountNumber() << endl;
            cout << "Your account balance: " << customer.getAccountBalance() << endl;

            double withdrawalAmount;
            cout << "Enter the withdrawal amount: ";
            cin >> withdrawalAmount;

            customer.withdraw(withdrawalAmount);
            break;
        }
        case 4: { 
            system("cls"); 
            cout << "Enter your account password: ";
            string password = maskInput(20);

            if (!customer.authenticatePassword(password)){
                cout << "Invalid password. Access denied." << endl;
                break;
            }

            cout << "Your account number: " << customer.getAccountNumber() << endl;
            cout << "Your account balance: " << customer.getAccountBalance() << endl;

            double depositAmount;
            cout << "Enter the deposit amount: ";
            cin >> depositAmount;

            customer.deposit(depositAmount);
            break;
        }
        case 5:
            system("cls");
            cout << "Exiting customer menu." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}

void employeeMenu(const string& employeeUsername, Bank& bank){
    int choice;
    cout << "Welcome, " << employeeUsername << "!" <<endl;
    cout << "1. Deposit" << endl;
    cout << "2. Withdraw" << endl;
    cout << "3. Currency Exchange" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    system("cls");

    switch (choice){
        case 1: {
            string customerAccountNumber;
            cout << "Enter customer account number: ";
            cin >> customerAccountNumber;
            double depositAmount;
            cout << "Enter the deposit amount: ";
            cin >> depositAmount;
            bank.deposit(customerAccountNumber, depositAmount);
            cout << "Deposit successful!" << endl;
            break;
        }
        case 2: {
            string customerAccountNumber;
            cout << "Enter customer account number: ";
            cin >> customerAccountNumber;

            ifstream inFile("customer_accounts.txt");
            ofstream outFile("temp_customer_accounts.txt");

            if (!inFile.is_open() || !outFile.is_open()){
                cout << "Error: Unable to open customer accounts file.\n";
                break;
            }

            string fileAccountNumber, fileUsername, filePassword;
            double fileBalance;
            bool found = false;

            while (inFile >> fileAccountNumber >> fileUsername >> filePassword >> fileBalance){
                if (fileAccountNumber == customerAccountNumber){
                    found = true;
                    double withdrawAmount;
                    cout << "Enter the withdrawal amount: ";
                    cin >> withdrawAmount;
                    if (withdrawAmount > fileBalance){
                        cout << "Insufficient funds." << endl;
                    } else {
                        fileBalance -= withdrawAmount;
                        cout << "Withdrawal of " << withdrawAmount << " Ksh successful!" << endl;
                        cout << "Remaining balance: " << fileBalance << " Ksh" <<endl;
                    }
                }
                //write the updated account information to the temp file
                outFile << fileAccountNumber << " " << fileUsername << " " << filePassword << " " << fileBalance << endl;
            }

            inFile.close();
            outFile.close();

            //remove the old file
            remove("customer_accounts.txt");

            //rename temp file to original file
            rename("temp_customer_accounts.txt", "customer_accounts.txt");

            if (!found){
                cout << "Customer acount not found." <<endl;
            }
            break;
        }
        case 3:
            currencyExchange();
            break;
        case 4:
            cout << "Exiting employee menu." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
    }
}


void Customer::menu(Bank& bank){
    customerMenu(*this);
}

void mainMenu() {
    Bank bank; // Create Bank instance

    int choice1, choice2, choice3;
    string adminName;
    string customerUsername;
    string employeeUsername;
    
    do {
        std::cout << "Welcome to Ventura Bank Management System\n";
        std::cout << "Do you want to?\n";
        std::cout << "1. Login\n";
        std::cout << "2. Sign up\n";
        std::cout << "3. Exit\n";
        std::cin >> choice1;

        switch (choice1) {
            case 1:
                system("cls");
                std::cout << "Welcome to Ventura Bank Management System\n";
                std::cout << "1. Admin Login\n";
                std::cout << "2. Employee Login\n";
                std::cout << "3. Customer Login\n";
                std::cout << "4. Exit\n";
                std::cout << "Enter your choice: ";
                std::cin >> choice2;

                switch (choice2) {
                    case 1:{
                        system("cls");
                        std::cout << "Enter admin username: ";
                        std::cin >> adminName;
                        std::cout << "Enter admin password: ";
                        string adminPasscode = maskInput(20);
                        system("cls");

                        if (adminName == "ventura" && adminPasscode == "adminpassword8") {
                            std::cout << "Login Successful";
                            Admin admin(adminName, adminPasscode);
                            admin.menu(bank);
                        } else {
                            std::cout << "Login Unsuccessful. Try again!" << endl;
                            mainMenu();
                        }
                        break;
                    }
                    case 2:{
                        system("cls");
                        std::cout << "Enter your username: ";
                        std::cin >> employeeUsername;
                        std::cout << "Enter your username: ";
                        string employeePassword = maskInput(20);
                        system("cls");

                        if (employeeLogin(employeeUsername, employeePassword)){
                            std::cout << "Login successful!" << endl;
                            employeeMenu(employeeUsername, bank);
                        } else {
                            std::cout << "Invalid username or password. Please try again" << endl;
                        }
                        break;
                    }
                    case 3:
                        system("cls");
                        std::cout << "Enter your username: ";
                        std::cin >> customerUsername;
                        std::cout << "Enter your password: ";
                        string customerPassword = maskInput(20);
                        system("cls");

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
                system("cls");
                std::cout << "Select one\n";
                std::cout << "1. Create employee\n";
                std::cout << "2. Create customer\n";
                std::cout << "3. Return to main menu\n";
                std::cin >> choice3;
                system("cls");

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
                system("cls");
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