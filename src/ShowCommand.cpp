#include "ShowCommand.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;


std::string formatSize(uintmax_t size) {
    if (size == 0) {
        return "0 B";
    }

    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double displaySize = static_cast<double>(size);

    while (displaySize >= 1024 && unitIndex < 4) {
        displaySize /= 1024;
        unitIndex++;
    }

    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << displaySize << " " << units[unitIndex];
    return ss.str();
}


class FileSystemNode {
protected:
    std::string fileName;
    uintmax_t size = 0;

public:
    explicit FileSystemNode(const std::string& name) : fileName(name) {}
    virtual ~FileSystemNode() = default;

    virtual uintmax_t getSize() const = 0;
    virtual void print(const std::string& prefix = "", bool isLast = true, bool isRoot = false) const = 0;
    std::string getName() const { return fileName; }
};


class File final : public FileSystemNode {
public:
    File(const std::string& name, uintmax_t size) : FileSystemNode(name) {
        this->size = size;
    }

    uintmax_t getSize() const override { return size; }

    void print(const std::string& prefix = "", bool isLast = true, bool isRoot = false) const override {
        std::cout << prefix << (isRoot ? "" : (isLast ? "└── " : "├── "))
                  << fileName << " (" << formatSize(size) << ")\n";
    }
};


class Directory final : public FileSystemNode {
public:
    std::vector<FileSystemNode*> children;

    explicit Directory(const std::string& name) : FileSystemNode(name) {}

    ~Directory() override {
        for (auto const child : children) {
            delete child;
        }
    }

    void addChild(FileSystemNode* child) {
        children.push_back(child);
    }

    uintmax_t getSize() const override {
        uintmax_t total = 0;
        for (auto const child : children) {
            total += child->getSize();
        }
        return total;
    }

    void print(const std::string& prefix = "", bool isLast = true, bool isRoot = false) const override {
        std::cout << prefix << (isRoot ? "" : (isLast ? "└── " : "├── "))
                  << fileName << "/\n";

        std::string newPrefix = prefix + (isRoot ? "" : (isLast ? "    " : "│   "));
        for (size_t i = 0; i < children.size(); ++i) {
            bool lastChild = (i == children.size() - 1);
            children[i]->print(newPrefix, lastChild);
        }
    }
};


Directory* buildTree(const fs::path& currentPath, int currentDepth, int maxDepth, const std::vector<std::string>& excludePatterns) {
    auto* dir = new Directory(currentPath.filename().string());

    if (maxDepth != -1 && currentDepth >= maxDepth) {
        return dir;
    }

    std::vector<fs::directory_entry> entries;
    try {
        for (const auto& entry : fs::directory_iterator(currentPath)) {
            std::string name = entry.path().filename().string();
            bool excluded = false;
            for (const auto& pattern : excludePatterns) {
                if (name == pattern) {
                    excluded = true;
                    break;
                }
            }
            if (!excluded) {
                entries.push_back(entry);
            }
        }
    } catch (fs::filesystem_error& e) {
        std::cerr << "Error accessing " << currentPath << ": " << e.what() << std::endl;
        return dir; 
    }


    std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
        return a.path().filename().string() < b.path().filename().string();
    });

    for (const auto& entry : entries) {
        try {
            if (fs::is_directory(entry.symlink_status())) {
                dir->addChild(buildTree(entry.path(), currentDepth + 1, maxDepth, excludePatterns));
            } else if (fs::is_regular_file(entry.symlink_status())) {
                dir->addChild(new File(entry.path().filename().string(), fs::file_size(entry.path())));
            }
        } catch (fs::filesystem_error& e) {
            dir->addChild(new File(entry.path().filename().string() + " [error]", 0));
        }
    }

    return dir;
}


void ShowCommand::execute(const std::string& path, const ShowOptions& options) const {
    fs::path targetPath = path.empty() ? fs::current_path() : fs::path(path);

    if (!fs::exists(targetPath)) {
        std::cerr << "Error: Path does not exist: " << targetPath << std::endl;
        return;
    }

    if (!fs::is_directory(targetPath)) {
        std::cerr << "Error: Path is not a directory: " << targetPath << std::endl;
        return;
    }

    Directory* root = buildTree(targetPath, 0, options.maxDepth, options.excludePatterns);

    std::cout << "Directory tree for: " << fs::absolute(targetPath).string() << "\n";
    root->print("", true, true);

    std::cout << "\nTotal size: " << formatSize(root->getSize()) << std::endl;

    delete root;
}
