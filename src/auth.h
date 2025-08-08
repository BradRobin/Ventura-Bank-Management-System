#ifndef AUTH_H
#define AUTH_H

#include <string>

class Auth {
public:
    explicit Auth(const std::string& credential_file);
    bool registerUser(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
private:
    std::string credFile;
    std::string hashPasswordWithSalt(const std::string& password, const std::string& salt);
    std::string generateSalt(int len = 16);
};

#endif // AUTH_H
