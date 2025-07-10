#pragma once
#include "commandInputStrategy.h"
#include <fstream>

class FileInputStrategy : public CommandInputStrategy {
public:
    FileInputStrategy(const std::string& filename);
    bool hasNextCommand() override;
    std::string getNextCommand() override;
private:
    std::ifstream file;
};