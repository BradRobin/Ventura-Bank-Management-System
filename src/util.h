#ifndef UTIL_H
#define UTIL_H

#include <string>

void clear_console();
bool atomic_write(const std::string& path, const std::string& data);
std::string read_file(const std::string& path);

#endif // UTIL_H
