#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#include <string>
#include <vector>

class MoveCommand {
public:
    void execute(const std::vector<std::string>& args) const;
};

#endif // MOVECOMMAND_H
