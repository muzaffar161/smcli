#include "SearchCommand.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;

// ANSI Color Codes
const std::string RESET = "\033[0m";
const std::string BOLD = "\033[1m";
const std::string BLUE = "\033[34m";
const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string YELLOW = "\033[33m";
const std::string CYAN = "\033[36m";
const std::string GRAY = "\033[90m";
const std::string HIGHLIGHT = "\033[43;30m"; 


bool SearchCommand::isImageFile(const std::string& extension) const {
    static const std::set<std::string> imageExtensions = {
        ".jpg", ".jpeg", ".png", ".gif", ".bmp", ".tiff", ".webp", ".svg", ".heif", ".heic"
    };
    return imageExtensions.count(extension);
}


bool SearchCommand::isVideoFile(const std::string& extension) const {
    static const std::set<std::string> videoExtensions = {
        ".mp4", ".mov", ".avi", ".mkv", ".wmv", ".flv", ".webm", ".m4v", ".3gp"
    };
    return videoExtensions.count(extension);
}


void printHighlighted(const std::string& text, const std::string& query, const std::string& baseColor) {
    if (query.empty()) {
        std::cout << baseColor << text << RESET;
        return;
    }

    std::string lowerText = text;
    std::string lowerQuery = query;
    std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(), ::tolower);
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    size_t pos = 0;
    size_t lastPos = 0;
    while ((pos = lowerText.find(lowerQuery, lastPos)) != std::string::npos) {
        std::cout << baseColor << text.substr(lastPos, pos - lastPos) << RESET;
        std::cout << HIGHLIGHT << text.substr(pos, query.length()) << RESET;
        lastPos = pos + query.length();
    }
    std::cout << baseColor << text.substr(lastPos) << RESET;
}

void SearchCommand::execute(const std::vector<std::string>& args) const {
    if (args.empty()) {
        std::cerr << "Usage: smcli search <query> [--flags]" << std::endl;
        std::cerr << "Flags:" << std::endl;
        std::cerr << "  -f             : Search for files only" << std::endl;
        std::cerr << "  -fl            : Search for folders only" << std::endl;
        std::cerr << "  -img           : Search for image files only" << std::endl;
        std::cerr << "  -vid           : Search for video files only" << std::endl;
        std::cerr << "  --exact-name    : Search for exact name match (case-insensitive)" << std::endl;
        std::cerr << "  --depth <N>     : Limit search to N levels deep" << std::endl;
        std::cerr << "  --exclude <name>: Exclude files or folders with the given name" << std::endl;
        return;
    }

    std::string query = "";
    bool search_files = true;
    bool search_folders = true;
    bool search_images = false;
    bool search_videos = false;
    bool exact_name_match = false;
    int depth_limit = -1;
    std::vector<std::string> exclude_patterns;


    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "-f") {
            search_folders = false;
        } else if (args[i] == "-fl") {
            search_files = false;
        } else if (args[i] == "-img") {
            search_images = true;
            search_folders = false;
        } else if (args[i] == "-vid") {
            search_videos = true;
            search_folders = false;
        } else if (args[i] == "--exact-name") {
            exact_name_match = true;
        } else if (args[i] == "--depth" && i + 1 < args.size()) {
            depth_limit = std::stoi(args[++i]);
        } else if (args[i] == "--exclude" && i + 1 < args.size()) {
            exclude_patterns.push_back(args[++i]);
        } else if (query.empty() && args[i][0] != '-') {
            query = args[i];
        }
    }

    if (query.empty()) {
        std::cerr << "Error: Search query is required." << std::endl;
        return;
    }

    std::cout << "Searching for '" << YELLOW << query << RESET << "'..." << std::endl;
    
    std::vector<fs::path> found_items;
    std::set<std::string> inaccessible_directories;

    auto process_entry = [&](const fs::directory_entry& entry, int current_depth) {
        try {
            fs::path p = entry.path();
            std::string item_name = p.filename().string();
            

            for (const auto& pattern : exclude_patterns) {
                if (item_name == pattern) return false;
            }

            std::string lower_item = item_name;
            std::string lower_query = query;
            std::transform(lower_item.begin(), lower_item.end(), lower_item.begin(), ::tolower);
            std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

            bool matches = exact_name_match ? (lower_item == lower_query) : (lower_item.find(lower_query) != std::string::npos);

            if (matches) {
                if (fs::is_regular_file(entry.status()) && search_files) {
                    std::string ext = p.extension().string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    if ((!search_images || isImageFile(ext)) && (!search_videos || isVideoFile(ext))) {
                        found_items.push_back(p);
                    }
                } else if (fs::is_directory(entry.status()) && search_folders) {
                    found_items.push_back(p);
                }
            }
        } catch (...) {}
        return true;
    };

    try {
        if (depth_limit == 0) {
            for (const auto& entry : fs::directory_iterator(fs::current_path(), fs::directory_options::skip_permission_denied)) {
                (void)process_entry(entry, 0);
            }
        } else {
            for (fs::recursive_directory_iterator it(fs::current_path(), fs::directory_options::skip_permission_denied), end; it != end; ++it) {
                if (depth_limit != -1 && it.depth() > depth_limit) {
                    it.disable_recursion_pending();
                    continue;
                }
                
                if (!process_entry(*it, it.depth())) {
                    it.disable_recursion_pending();
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << RED << "Error during search: " << e.what() << RESET << std::endl;
    }


    if (found_items.empty()) {
        std::cout << "No items found." << std::endl;
    } else {
        std::cout << "\n" << BOLD << "Found " << found_items.size() << " items:" << RESET << std::endl;
        for (const auto& item : found_items) {
            std::string parent = item.parent_path().string();
            std::string name = item.filename().string();
            
            std::cout << GRAY << parent << (parent.empty() ? "" : "/") << RESET;
            
            std::string color = fs::is_directory(item) ? BLUE : (isImageFile(item.extension().string()) || isVideoFile(item.extension().string()) ? GREEN : RESET);
            if (fs::is_directory(item)) color += BOLD;

            printHighlighted(name, query, color);
            if (fs::is_directory(item)) std::cout << BLUE << BOLD << "/" << RESET;
            std::cout << std::endl;
        }
    }
}
