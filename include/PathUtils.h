#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

fs::path generate_new_path(const fs::path& destination_path);
fs::path get_binary_path();
std::vector<std::string> load_ignore_patterns();
void add_global_ignore_pattern(const std::string& pattern);
bool wildcard_match(const std::string& text, const std::string& pattern);

#endif // PATHUTILS_H
