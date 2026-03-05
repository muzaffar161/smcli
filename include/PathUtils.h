#ifndef PATHUTILS_H
#define PATHUTILS_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

fs::path generate_new_path(const fs::path& destination_path);

#endif // PATHUTILS_H
