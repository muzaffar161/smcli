#ifndef COPYCOMMAND_H
#define COPYCOMMAND_H

#include <string>
#include <vector>

class CopyCommand {
public:
    void execute(const std::vector<std::string>& args) const;
};

#endif // COPYCOMMAND_H
