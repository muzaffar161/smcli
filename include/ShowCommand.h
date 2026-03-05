#ifndef SHOWCOMMAND_H
#define SHOWCOMMAND_H

#include <string>
#include <vector>

struct ShowOptions {
    int maxDepth = -1; // -1 for infinite
    std::vector<std::string> excludePatterns;
};

class ShowCommand {
public:
    void execute(const std::string& path, const ShowOptions& options = {}) const;
};

#endif // SHOWCOMMAND_H
