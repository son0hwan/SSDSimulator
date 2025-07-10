#pragma once
#include "commandInputStrategy.h"

class ConsoleInputStrategy : public CommandInputStrategy {
public:
    bool hasNextCommand() override;
    std::string getNextCommand() override;
};