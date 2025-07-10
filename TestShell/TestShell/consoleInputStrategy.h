#pragma once
#include "commandInputStrategy.h"

class ConsoleInputStrategy : public CommandInputStrategy {
public:
    bool hasNextCommand() override;
    std::string getNextCommand() override;
    void print(const std::string& cmdName, int status) override;
    void prePrint(const std::string& cmdName) override;
};