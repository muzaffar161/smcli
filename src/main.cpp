#include <iostream>
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ShowCommand.h"
#include "CopyCommand.h"
#include "MoveCommand.h"
#include "ImportCommand.h"
#include "SearchCommand.h"
#include "DUCommand.h"
#include "PathUtils.h"

void printUsage() {
    std::cout << "Usage: smcli <command> [options]\n"
              << "\n"
              << "Commands:\n"
              << "  show [path] [--depth <N>] [--exclude <name>] [--no-ignore]  Display a tree map.\n"
              << "  du [path]                                     Show disk usage with progress bars.\n"
              << "  copy | cp <source> to <dest> [as <name>]     Copy a file or directory.\n"
              << "  move | mv <source> to <dest> [as <name>]     Move/rename a file or directory.\n"
              << "  import | get <source> [as <name>]            Import to current location.\n"
              << "  search | find <query> [options]              Search for files and folders.\n"
              << "  --exclude-add-global <pattern>               Add pattern to global ignore file.\n"
              << "  help                                         Show this help message.\n"
              << "\n"
              << "Common Options:\n"
              << "  --exclude <name>: Exclude files or dirs (can be used multiple times).\n"
              << "  --no-ignore    : Show all files, bypassing .smcliignore and global ignore.\n";
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    
    // Handle global ignore addition
    if (command == "--exclude-add-global") {
        if (argc < 3) {
            std::cerr << "Error: Pattern required." << std::endl;
            return 1;
        }
        add_global_ignore_pattern(argv[2]);
        return 0;
    }

    if (command == "help" || command == "--help") {
        printUsage();
        return 0;
    }

    std::vector<std::string> args;
    for (int i = 2; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    if (command == "show") {
        ShowCommand showCmd;
        std::string path = "";
        ShowOptions options;

        for (size_t i = 0; i < args.size(); ++i) {
            if (args[i] == "--depth" && i + 1 < args.size()) {
                try { options.maxDepth = std::stoi(args[++i]); } catch (...) {}
            } else if (args[i] == "--exclude" && i + 1 < args.size()) {
                options.excludePatterns.push_back(args[++i]);
            } else if (args[i] == "--no-ignore") {
                options.noIgnore = true;
            } else if (path.empty() && args[i][0] != '-') {
                path = args[i];
            }
        }
        showCmd.execute(path, options);
    } else if (command == "du") {
        DUCommand duCmd;
        duCmd.execute(args);
    } else if (command == "copy" || command == "cp") {
        CopyCommand copyCmd;
        copyCmd.execute(args);
    } else if (command == "move" || command == "mv") {
        MoveCommand moveCmd;
        moveCmd.execute(args);
    } else if (command == "import" || command == "get") {
        ImportCommand importCmd;
        importCmd.execute(args);
    } else if (command == "search" || command == "find") {
        SearchCommand searchCmd;
        searchCmd.execute(args);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        printUsage();
        return 1;
    }

    return 0;
}
