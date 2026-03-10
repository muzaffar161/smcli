#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H

#include <string>
#include <vector>
#include <cstdint>

struct ShowOptions {
    int maxDepth = -1; 
    std::vector<std::string> excludePatterns;
};

// Reusable formatting utility
std::string formatSize(uintmax_t size);

class ShowCommand {
public:
    void execute(const std::string& path, const ShowOptions& options = {}) const;
};

#endif // SHOWCOMMAND_H
