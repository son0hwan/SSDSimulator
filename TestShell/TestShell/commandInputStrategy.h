#pragma once
#include <string>

class CommandInputStrategy {
public:
    virtual bool hasNextCommand() = 0;
    virtual std::string getNextCommand() = 0;
};