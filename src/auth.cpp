#include "auth.h"
#include "util.h"
#include <fstream>
#include <sstream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iomanip>
#include <iostream>

static std::string to_hex(const unsigned char* data, size_t len){
    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for(size_t i=0;i<len;i++) ss << std::setw(2) << (int)data[i];
    return ss.str();
}

Auth::Auth(const std::string& credential_file) : credFile(credential_file) {}

std::string Auth::generateSalt(int len) {
    unsigned char buf[32];
    if(RAND_bytes(buf, len) != 1) {
        // fallback: pseudo-random
        for(int i=0;i<len;i++) buf[i] = (unsigned char)(rand() % 256);
    }
    return to_hex(buf, len);
}

std::string Auth::hashPasswordWithSalt(const std::string& password, const std::string& salt) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if(!ctx) return "";
    if(1 != EVP_DigestInit_ex(ctx, EVP_sha256(), NULL)) { EVP_MD_CTX_free(ctx); return ""; }
    EVP_DigestUpdate(ctx, salt.data(), salt.size());
    EVP_DigestUpdate(ctx, password.data(), password.size());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);
    EVP_MD_CTX_free(ctx);
    return to_hex(hash, hash_len);
}

bool Auth::registerUser(const std::string& username, const std::string& password) {
    // check if exists
    std::ifstream ifs(credFile);
    if(ifs.is_open()) {
        std::string line;
        while(std::getline(ifs, line)) {
            if(line.rfind(username + ":", 0) == 0) {
                return false; // user exists
            }
        }
    }
    std::string salt = generateSalt(16);
    std::string hash = hashPasswordWithSalt(password, salt);
    if(hash.empty()) return false;
    std::string out = username + ":" + salt + ":" + hash + "\n";
    // append atomically
    std::string existing = read_file(credFile);
    existing += out;
    if(!atomic_write(credFile, existing)) {
        std::cerr << "Failed to write credentials file\n";
        return false;
    }
    return true;
}

bool Auth::login(const std::string& username, const std::string& password) {
    std::ifstream ifs(credFile);
    if(!ifs.is_open()) return false;
    std::string line;
    while(std::getline(ifs, line)) {
        if(line.rfind(username + ":", 0) == 0) {
            // format username:salt:hash
            auto p1 = line.find(':');
            auto p2 = line.find(':', p1+1);
            if(p1==std::string::npos || p2==std::string::npos) return false;
            std::string salt = line.substr(p1+1, p2 - (p1+1));
            std::string stored_hash = line.substr(p2+1);
            std::string computed = hashPasswordWithSalt(password, salt);
            return stored_hash == computed;
        }
    }
    return false;
}
