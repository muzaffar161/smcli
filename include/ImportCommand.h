#ifndef IMPORTCOMMAND_H
#define IMPORTCOMMAND_H

#include <string>
#include <vector>

class ImportCommand {
public:
    void execute(const std::vector<std::string>& args) const;
};

#endif // IMPORTCOMMAND_H
