#include "ImportCommand.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include "PathUtils.h"

namespace fs = std::filesystem;

void ImportCommand::execute(const std::vector<std::string>& args) const {
    if (args.empty()) {
        std::cerr << "Usage: smcli import <source> [as <new_name>]" << std::endl;
        return;
    }

    fs::path source_path = args[0];
    fs::path current_dir = fs::current_path();
    std::string new_name = "";

    if (args.size() > 2 && args[1] == "as") {
        new_name = args[2];
    }

    if (!fs::exists(source_path)) {
        std::cerr << "Error: Source path '" << source_path << "' does not exist." << std::endl;
        return;
    }

    try {
        fs::path final_destination;

        if (new_name.empty()) {
            final_destination = current_dir / source_path.filename();
        } else {
            final_destination = current_dir / new_name;
        }
        
        if (fs::exists(final_destination)) {
            if (new_name.empty()) { 
                final_destination = generate_new_path(final_destination); 
                std::cout << "Destination '" << (current_dir / source_path.filename()) << "' already exists. Importing to '" << final_destination.filename() << "'." << std::endl;
            } else {
                 std::cerr << "Error: Destination '" << final_destination << "' already exists." << std::endl;
                 return;
            }
        }
        
        fs::copy_options options = fs::copy_options::recursive;
        fs::copy(source_path, final_destination, options);
        std::cout << "Successfully imported '" << source_path << "' to '" << final_destination << "'." << std::endl;

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}
