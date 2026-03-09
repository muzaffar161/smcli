#include "PathUtils.h"
#include <iostream>
#include <string>
#include <sstream>

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