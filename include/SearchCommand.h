#ifndef SEARCHCOMMAND_H
#define SEARCHCOMMAND_H

#include <string>
#include <vector>
#include <set>

class SearchCommand {
public:
    void execute(const std::vector<std::string>& args) const;

private:
    bool isImageFile(const std::string& extension) const;
    bool isVideoFile(const std::string& extension) const;
};

#endif // SEARCHCOMMAND_H
