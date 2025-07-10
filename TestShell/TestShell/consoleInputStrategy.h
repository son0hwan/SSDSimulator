#pragma once
#include "commandInputStrategy.h"

class ConsoleInputStrategy : public CommandInputStrategy {
public:
    bool hasNextCommand() override;
    std::string getNextCommand() override;
    void print(const std::string& cmdName, const std::string& status) override;
};