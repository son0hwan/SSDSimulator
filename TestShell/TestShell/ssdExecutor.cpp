#include <cstdlib>
#include <string>
#include <iostream>
#include "executor.h"
#include "common.h"

class SSDExecutor : public Executor {
    std::string readFromSSD(int address) override {
        std::string command = "ssd.exe R " + std::to_string(address);
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cout << "read fail : return code = " << result << std::endl;
            return FAIL;
        }
        return PASS;
    }

    std::string writeToSSD(int address, int value) override {
        char hexStr[10];
        std::snprintf(hexStr, sizeof(hexStr), "0x%X", value);
        std::string command = "ssd.exe W " + std::to_string(address) + " " + hexStr;
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cout << "write fail : return code = " << result << std::endl;
            return FAIL;
        }
        return PASS;
    }
};
