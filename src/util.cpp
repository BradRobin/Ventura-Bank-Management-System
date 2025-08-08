#include "util.h"
#include <fstream>
#include <cstdio>
#include <iostream>

void clear_console() {
    // ANSI escape sequence to clear screen and move cursor to home
    std::cout << "\x1B[2J\x1B[H";
}

bool atomic_write(const std::string& path, const std::string& data) {
    std::string tmp = path + ".tmp";
    std::ofstream ofs(tmp, std::ios::binary);
    if(!ofs) return false;
    ofs << data;
    ofs.close();
    if(std::rename(tmp.c_str(), path.c_str()) != 0) {
        std::remove(tmp.c_str());
        return false;
    }
    return true;
}

std::string read_file(const std::string& path) {
    std::ifstream ifs(path);
    if(!ifs.is_open()) return std::string();
    std::string contents((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    return contents;
}
