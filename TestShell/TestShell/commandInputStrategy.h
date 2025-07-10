#pragma once
#include <string>

class CommandInputStrategy {
public:
    virtual bool hasNextCommand() = 0;
    virtual std::string getNextCommand() = 0;
    virtual void print(const std::string& cmdName, const std::string& status) = 0;
};