#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>  // for std::stol

class SsdCmdParser {
public:
    SsdCmdInterface* getCommand(const std::vector<std::string>& args);

private:
    const int NUM_OF_READ_ARGS = 2;
    const int NUM_OF_WRITE_ARGS = 3;

    const int MIN_ADDRESS = 0;
    const int MAX_ADDRESS = 99;
    const int INVALID_ADDRESS = -1;

    bool isHexString(const std::string& address);
    bool isLbaString(const std::string& address);
};