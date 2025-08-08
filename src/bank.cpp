#include "bank.h"
#include <fstream>
#include <iostream>
#include <sstream>

Bank::Bank(const std::string& accounts_file) : accountsFile(accounts_file) {}

void Bank::showAccountsForUser(const std::string& username) const {
    std::ifstream ifs(accountsFile);
    if(!ifs.is_open()) {
        std::cout << "No accounts file found.\n";
        return;
    }
    std::string line;
    bool found = false;
    while(std::getline(ifs, line)) {
        // expected CSV: account_number,username,balance,currency
        std::istringstream ss(line);
        std::string acc, user, bal, cur;
        if(!std::getline(ss, acc, ',')) continue;
        if(!std::getline(ss, user, ',')) continue;
        if(!std::getline(ss, bal, ',')) continue;
        if(!std::getline(ss, cur, ',')) cur = "KSH";
        if(user == username) {
            std::cout << "Account: " << acc << " | Balance: " << bal << " " << cur << "\n";
            found = true;
        }
    }
    if(!found) std::cout << "No accounts for user: " << username << "\n";
}
