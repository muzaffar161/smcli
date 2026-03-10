#include "PathUtils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;


fs::path generate_new_path(const fs::path& destination_path) {
    if (!fs::exists(destination_path)) {
        return destination_path;
    }

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
            } catch (const std::invalid_argument&) {

            }
        } else if (potential_num.length() == 0) {
             stem_base = stem.substr(0, pos);
             copy_num = 2;
        }
    }


    fs::path new_path;
    do {
        std::string new_stem;
        if (copy_num == 1) {
            new_stem = stem_base + " copy";
        } else {
            new_stem = stem_base + " copy " + std::to_string(copy_num);
        }
        new_path = parent_path / (new_stem + extension);
        copy_num++;
    } while (fs::exists(new_path));

    return new_path;
}

std::vector<std::string> load_ignore_patterns() {
    std::vector<std::string> patterns;
    fs::path ignore_file = fs::current_path() / ".smcliignore";
    
    if (!fs::exists(ignore_file)) {
        return patterns;
    }

    std::ifstream file(ignore_file);
    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (!line.empty() && line[0] != '#') {
            patterns.push_back(line);
        }
    }
    return patterns;
}

bool wildcard_match(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return text.empty();
    if (pattern == "*") return true;

    // Simple prefix check for dir/*
    if (pattern.size() > 2 && pattern.substr(pattern.size() - 2) == "/*") {
        std::string prefix = pattern.substr(0, pattern.size() - 2);
        return text == prefix || (text.size() > prefix.size() && text.substr(0, prefix.size() + 1) == prefix + "/");
    }

    // Simple extension check for *.ext
    if (pattern.size() > 2 && pattern.substr(0, 2) == "*.") {
        std::string ext = pattern.substr(1);
        if (text.size() < ext.size()) return false;
        return text.substr(text.size() - ext.size()) == ext;
    }

    return text == pattern;
}
