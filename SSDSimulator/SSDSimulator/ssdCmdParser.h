#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>  // for std::stol

class SsdCmdInterface;

class SsdCmdParser {
public:
    SsdCmdInterface* getCommand(const std::vector<std::string>& args);

private:
    const int NUM_OF_READ_ARGS = 2;
    const int NUM_OF_WRITE_ARGS = 3;

    bool isHexString(const std::string& address);
    bool isRightLba(const std::string& address);

    bool isReadCmd(const std::vector<std::string>& args);
    bool isWriteCmd(const std::vector<std::string>& args);
};