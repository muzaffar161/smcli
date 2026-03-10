#include "PathUtils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <limits.h>
#include <stdlib.h>
#else
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#endif

namespace fs = std::filesystem;


fs::path get_binary_path() {
    char buffer[PATH_MAX];
#ifdef _WIN32
    GetModuleFileNameA(NULL, buffer, sizeof(buffer));
    return fs::path(buffer).parent_path();
#elif defined(__APPLE__)
    uint32_t size = sizeof(buffer);
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        char real_p[PATH_MAX];
        if (realpath(buffer, real_p)) {
            return fs::path(real_p).parent_path();
        }
        return fs::path(buffer).parent_path();
    }
    return fs::current_path();
#else
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        char real_p[PATH_MAX];
        if (realpath(buffer, real_p)) {
            return fs::path(real_p).parent_path();
        }
        return fs::path(buffer).parent_path();
    }
    return fs::current_path();
#endif
}

fs::path get_global_ignore_path() {
#ifdef _WIN32
    const char* userprofile = std::getenv("USERPROFILE");
    if (userprofile) return fs::path(userprofile) / ".smcliignore-global";
#else
    const char* home = std::getenv("HOME");
    if (home) return fs::path(home) / ".smcliignore-global";
#endif
    return get_binary_path() / ".smcliignore-global";
}

fs::path generate_new_path(const fs::path& destination_path) {
    if (!fs::exists(destination_path)) return destination_path;
    fs::path parent_path = destination_path.parent_path();
    std::string stem = destination_path.stem().u8string();
    std::string extension = destination_path.extension().u8string();
    std::string stem_base = stem;
    int copy_num = 1;
    size_t pos = stem.rfind(" copy");
    if (pos != std::string::npos) {
        std::string potential_num = stem.substr(pos + 5);
        if (potential_num.length() > 0 && potential_num[0] == ' ') {
            try {
                copy_num = std::stoi(potential_num.substr(1));
                stem_base = stem.substr(0, pos);
                copy_num++;
            } catch (...) {}
        } else if (potential_num.length() == 0) {
             stem_base = stem.substr(0, pos);
             copy_num = 2;
        }
    }
    fs::path new_path;
    do {
        std::string new_stem = (copy_num == 1) ? (stem_base + " copy") : (stem_base + " copy " + std::to_string(copy_num));
        new_path = parent_path / (new_stem + extension);
        copy_num++;
    } while (fs::exists(new_path));
    return new_path;
}

std::vector<std::string> load_from_file(const fs::path& p) {
    std::vector<std::string> patterns;
    std::ifstream file(p);
    if (!file.is_open()) return patterns;
    std::string line;
    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (!line.empty() && line[0] != '#') patterns.push_back(line);
    }
    return patterns;
}

void ensure_global_ignore_exists(const fs::path& p) {
    if (fs::exists(p)) return;
    std::ofstream file(p);
    if (!file.is_open()) return;
    file << "# SmCLI Global Ignore Patterns\n"
         << ".git\nnode_modules\nobj\nbin\n.DS_Store\n.env\n.vscode\n.idea\ndist\nbuild\n*.o\n*.exe\n*.so\n*.a\n";
    file.close();
}

std::vector<std::string> load_ignore_patterns() {
    std::vector<std::string> patterns;
    fs::path global_ignore = get_global_ignore_path();
    ensure_global_ignore_exists(global_ignore);
    auto global_patterns = load_from_file(global_ignore);
    patterns.insert(patterns.end(), global_patterns.begin(), global_patterns.end());

    fs::path current = fs::current_path();
    while (true) {
        fs::path p = current / ".smcliignore";
        if (fs::exists(p)) {
            auto local_patterns = load_from_file(p);
            patterns.insert(patterns.end(), local_patterns.begin(), local_patterns.end());
            break;
        }
        if (current.has_parent_path() && current != current.root_path()) current = current.parent_path();
        else break;
    }
    std::sort(patterns.begin(), patterns.end());
    patterns.erase(std::unique(patterns.begin(), patterns.end()), patterns.end());
    return patterns;
}

void add_global_ignore_pattern(const std::string& pattern) {
    fs::path global_ignore = get_global_ignore_path();
    ensure_global_ignore_exists(global_ignore);
    auto patterns = load_from_file(global_ignore);
    if (std::find(patterns.begin(), patterns.end(), pattern) != patterns.end()) {
        std::cout << "Pattern '" << pattern << "' is already in global ignore." << std::endl;
        return;
    }
    std::ofstream file(global_ignore, std::ios_base::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open global ignore file: " << global_ignore << std::endl;
        return;
    }
    file << pattern << "\n";
    file.close();
    std::cout << "Added '" << pattern << "' to global ignore (" << global_ignore.u8string() << ")." << std::endl;
}

bool wildcard_match(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return text.empty();
    std::string norm_pattern = pattern;
    if (norm_pattern.size() > 1 && (norm_pattern.back() == '/' || norm_pattern.back() == '\\')) norm_pattern.pop_back();
    if (norm_pattern == "*") return true;

    std::string lower_text = text;
    std::string lower_pattern = norm_pattern;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), ::tolower);
    std::transform(lower_pattern.begin(), lower_pattern.end(), lower_pattern.begin(), ::tolower);

    if (lower_pattern.back() == '*') {
        std::string prefix = lower_pattern.substr(0, lower_pattern.size() - 1);
        return lower_text.substr(0, prefix.size()) == prefix;
    }
    if (lower_pattern.size() > 2 && lower_pattern.substr(0, 2) == "*.") {
        std::string ext = lower_pattern.substr(1);
        if (lower_text.size() < ext.size()) return false;
        return lower_text.substr(lower_text.size() - ext.size()) == ext;
    }
    return lower_text == lower_pattern;
}
