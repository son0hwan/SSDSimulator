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
        return getFirstLineFromFile(OUTPUT_FILE_NAME);
    }

    std::string writeToSSD(int address, unsigned int value) override {
        char hexStr[11];
        std::snprintf(hexStr, sizeof(hexStr), "0x%X", value);
        std::string command = "ssd.exe W " + std::to_string(address) + " " + hexStr;
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cout << "write fail : return code = " << result << std::endl;
            return FAIL;
        }

        return getFirstLineFromFile(OUTPUT_FILE_NAME);
    }

    std::string eraseToSSD(int address, int size) override {
        std::string command = "ssd.exe E " + std::to_string(address) + " " + std::to_string(size);
        
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cout << "erase fail : return code = " << result << std::endl;
            return FAIL;
        }

        return getFirstLineFromFile(OUTPUT_FILE_NAME);
    }

    std::string flushToSSD() override {
        std::string command = "ssd.exe F";

        int result = std::system(command.c_str());
        if (result != 0) {
            std::cout << "flush fail : return code = " << result << std::endl;
            return FAIL;
        }

        return getFirstLineFromFile(OUTPUT_FILE_NAME);
    }
};
