#include "MoveCommand.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "PathUtils.h" 

namespace fs = std::filesystem;

void MoveCommand::execute(const std::vector<std::string>& args) const {
    if (args.size() < 3 || args[1] != "to") {
        std::cerr << "Usage: smcli move <source> to <destination> [as <new_name>]" << std::endl;
        return;
    }

    fs::path source_path = args[0];
    fs::path destination_path = args[2];
    std::string new_name = "";

    if (args.size() > 4 && args[3] == "as") {
        new_name = args[4];
    }

    if (!fs::exists(source_path)) {
        std::cerr << "Error: Source path '" << source_path << "' does not exist." << std::endl;
        return;
    }

    try {
        fs::path final_destination;

        if (fs::is_directory(destination_path)) {
            if (new_name.empty()) {
                final_destination = destination_path / source_path.filename();
            } else {
                final_destination = destination_path / new_name;
            }
        } else { 
             if (new_name.empty()) {
                final_destination = destination_path;
             } else {
                final_destination = destination_path.parent_path() / new_name;
             }
        }

        if (fs::exists(final_destination)) {
            if (new_name.empty()) {
                final_destination = generate_new_path(final_destination); 
                std::cout << "Destination '" << (destination_path / source_path.filename()) << "' already exists. Moving to '" << final_destination.filename() << "'." << std::endl;
            } else {
                 std::cerr << "Error: Destination '" << final_destination << "' already exists." << std::endl;
                 return;
            }
        }
        
        fs::rename(source_path, final_destination);
        std::cout << "Successfully moved '" << source_path << "' to '" << final_destination << "'." << std::endl;

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}
