#pragma once
#include "executor.h"
class TestShell {
public:
    TestShell(Executor* executor) : executor(executor) {}
    std::string read(int address) {
        return executor->readFromSSD(address);
    }
    std::string fullRead() {
        return "";
    }

private:
    Executor* executor;
};