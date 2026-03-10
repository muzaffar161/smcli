#ifndef SEARCHCOMMAND_H
#define SEARCHCOMMAND_H

#include <string>
#include <vector>
#include <set>
#include <filesystem>

namespace fs = std::filesystem;

class SearchCommand {
public:
    void execute(const std::vector<std::string>& args) const;

private:
    bool isImageFile(const std::string& extension) const;
    bool isVideoFile(const std::string& extension) const;
    bool isTextFile(const std::string& extension) const;
    bool fileContains(const fs::path& filePath, const std::string& query) const;
};

#endif // SEARCHCOMMAND_H
