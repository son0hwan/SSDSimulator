#pragma once
#include "commandInputStrategy.h"
#include <fstream>

class FileInputStrategy : public CommandInputStrategy {
public:
    FileInputStrategy(const std::string& filename);
    bool hasNextCommand() override;
    std::string getNextCommand() override;
    void print(const std::string& cmdName, int status) override;
    void prePrint(const std::string& cmdName) override;
private:
    std::ifstream file;
};