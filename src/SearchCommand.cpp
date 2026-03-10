#include "SearchCommand.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <set>
#include <chrono>
#include "UIUtils.h"
#include "PathUtils.h"

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

bool SearchCommand::isTextFile(const std::string& extension) const {
    static const std::set<std::string> textExtensions = {
        ".txt", ".md", ".cpp", ".h", ".js", ".ts", ".py", ".go", ".c", ".java", ".json", ".xml", ".html", ".css", ".sh", ".yml", ".yaml", ".ini", ".log", ".env", ""
    };
    return textExtensions.count(extension);
}

bool SearchCommand::fileContains(const fs::path& filePath, const std::string& query) const {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;
    
    std::string line;
    std::string lowerQuery = query;
    std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

    while (std::getline(file, line)) {
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);
        if (line.find(lowerQuery) != std::string::npos) return true;
    }
    return false;
}

uintmax_t parseSize(const std::string& sizeStr) {
    if (sizeStr.empty()) return 0;
    std::string numPart;
    std::string unitPart;
    for (char c : sizeStr) {
        if (std::isdigit(c)) numPart += c;
        else unitPart += c;
    }
    if (numPart.empty()) return 0;
    uintmax_t size = std::stoull(numPart);
    std::transform(unitPart.begin(), unitPart.end(), unitPart.begin(), ::toupper);
    if (unitPart == "KB" || unitPart == "K") size *= 1024;
    else if (unitPart == "MB" || unitPart == "M") size *= 1024 * 1024;
    else if (unitPart == "GB" || unitPart == "G") size *= 1024ULL * 1024 * 1024;
    return size;
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
        std::cerr << "Usage: smcli search <query> [options]" << std::endl;
        std::cerr << "Options:" << std::endl;
        std::cerr << "  -f                : Search for files only" << std::endl;
        std::cerr << "  -fl               : Search for folders only" << std::endl;
        std::cerr << "  -img              : Search for image files only" << std::endl;
        std::cerr << "  -vid              : Search for video files only" << std::endl;
        std::cerr << "  -c, --content     : Search inside file contents" << std::endl;
        std::cerr << "  --exact-name      : Exact name match (case-insensitive)" << std::endl;
        std::cerr << "  --depth <N>       : Limit search to N levels deep" << std::endl;
        std::cerr << "  --exclude <name>  : Exclude specific name (can be used multiple times)" << std::endl;
        std::cerr << "  --min-size <size> : Files larger than size (e.g. 10MB)" << std::endl;
        std::cerr << "  --max-size <size> : Files smaller than size" << std::endl;
        std::cerr << "  --newer-than <N>  : Modified in the last N days" << std::endl;
        std::cerr << "  --older-than <N>  : Modified more than N days ago" << std::endl;
        std::cerr << "  --no-ignore       : Bypass all ignore files" << std::endl;
        return;
    }

    std::string query = "";
    bool search_files = true;
    bool search_folders = true;
    bool search_images = false;
    bool search_videos = false;
    bool search_content = false;
    bool exact_name_match = false;
    int depth_limit = -1;
    std::vector<std::string> exclude_patterns;
    bool no_ignore = false;
    uintmax_t min_size = 0, max_size = 0;
    int newer_than_days = -1, older_than_days = -1;

    for (const auto& arg : args) {
        if (arg == "--no-ignore") {
            no_ignore = true;
            break;
        }
    }

    if (!no_ignore) {
        exclude_patterns = load_ignore_patterns();
    }

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
        } else if (args[i] == "-c" || args[i] == "--content") {
            search_content = true;
            search_folders = false;
        } else if (args[i] == "--no-ignore") {
            continue;
        } else if (args[i] == "--exact-name") {
            exact_name_match = true;
        } else if (args[i] == "--depth" && i + 1 < args.size()) {
            depth_limit = std::stoi(args[++i]);
        } else if (args[i] == "--exclude" && i + 1 < args.size()) {
            exclude_patterns.push_back(args[++i]);
        } else if (args[i] == "--min-size" && i + 1 < args.size()) {
            min_size = parseSize(args[++i]);
        } else if (args[i] == "--max-size" && i + 1 < args.size()) {
            max_size = parseSize(args[++i]);
        } else if (args[i] == "--newer-than" && i + 1 < args.size()) {
            newer_than_days = std::stoi(args[++i]);
        } else if (args[i] == "--older-than" && i + 1 < args.size()) {
            older_than_days = std::stoi(args[++i]);
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
    Spinner spinner;
    int items_processed = 0;

    auto now = std::chrono::system_clock::now();

    auto process_entry = [&](const fs::directory_entry& entry, int current_depth) {
        if (++items_processed % 100 == 0) {
            spinner.next("Scanning... [" + std::to_string(found_items.size()) + " matches found]");
        }
        try {
            fs::path p = entry.path();
            std::string item_name = p.filename().u8string();
            
            for (const auto& pattern : exclude_patterns) {
                if (wildcard_match(item_name, pattern)) return false;
            }

            if (min_size > 0 || max_size > 0) {
                if (!fs::is_regular_file(entry.status())) return true;
                uintmax_t fsize = fs::file_size(p);
                if (min_size > 0 && fsize < min_size) return true;
                if (max_size > 0 && fsize > max_size) return true;
            }

            if (newer_than_days != -1 || older_than_days != -1) {
                auto ftime = fs::last_write_time(p);
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                auto age_days = std::chrono::duration_cast<std::chrono::hours>(now - sctp).count() / 24;
                if (newer_than_days != -1 && age_days > newer_than_days) return true;
                if (older_than_days != -1 && age_days < older_than_days) return true;
            }

            std::string lower_item = item_name;
            std::string lower_query = query;
            std::transform(lower_item.begin(), lower_item.end(), lower_item.begin(), ::tolower);
            std::transform(lower_query.begin(), lower_query.end(), lower_query.begin(), ::tolower);

            bool name_matches = exact_name_match ? (lower_item == lower_query) : (lower_item.find(lower_query) != std::string::npos);
            bool content_matches = false;

            if (search_content && fs::is_regular_file(entry.status())) {
                std::string ext = p.extension().u8string();
                std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                if (isTextFile(ext)) {
                    content_matches = fileContains(p, query);
                }
            }

            if (name_matches || content_matches) {
                if (fs::is_regular_file(entry.status()) && search_files) {
                    std::string ext = p.extension().u8string();
                    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
                    if ((!search_images || isImageFile(ext)) && (!search_videos || isVideoFile(ext))) {
                        found_items.push_back(p);
                    }
                } else if (fs::is_directory(entry.status()) && search_folders && !search_content) {
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
                if (!process_entry(*it, it.depth())) it.disable_recursion_pending();
            }
        }
        spinner.clear();
    } catch (...) {
        spinner.clear();
    }

    if (found_items.empty()) {
        std::cout << "No items found." << std::endl;
    } else {
        std::cout << "\n" << BOLD << "Found " << found_items.size() << " items:" << RESET << std::endl;
        for (const auto& item : found_items) {
            std::string parent = item.parent_path().u8string();
            std::string name = item.filename().u8string();
            std::cout << GRAY << parent << (parent.empty() ? "" : "/") << RESET;
            std::string color = fs::is_directory(item) ? BLUE : (isImageFile(item.extension().u8string()) || isVideoFile(item.extension().u8string()) ? GREEN : RESET);
            if (fs::is_directory(item)) color += BOLD;
            printHighlighted(name, query, color);
            if (fs::is_directory(item)) std::cout << BLUE << BOLD << "/" << RESET;
            std::cout << std::endl;
        }
    }
}
