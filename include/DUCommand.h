#ifndef DUCOMMAND_H
#define DUCOMMAND_H

#include <string>
#include <vector>

class DUCommand {
public:
    void execute(const std::vector<std::string>& args) const;
};

#endif
