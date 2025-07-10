#pragma once
#include <string>
#include "common.h"

class CommandInputStrategy {
public:
    virtual bool hasNextCommand() = 0;
    virtual std::string getNextCommand() = 0;
    virtual void print(const std::string& cmdName, int status) = 0;
    virtual void prePrint(const std::string& cmdName) = 0;
};