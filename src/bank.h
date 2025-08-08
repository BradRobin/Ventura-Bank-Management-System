#ifndef BANK_H
#define BANK_H

#include <string>

class Bank {
public:
    explicit Bank(const std::string& accounts_file);
    void showAccountsForUser(const std::string& username) const;
private:
    std::string accountsFile;
};

#endif // BANK_H
