#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>  // for std::stol
#include "ssdInterface.h"

class SsdCmdParser {
public:
    SsdCmdInterface* getCommand(const std::vector<std::string>& args);

private:
    const int NUM_OF_READ_ARGS = 2;
    const int NUM_OF_WRITE_ARGS = 3;
    const int NUM_OF_ERASE_ARGS = 3;
    const int NUM_OF_FLUSH_ARGS = 1;

    SsdCmdInterface* handleReadCommand(const std::vector<std::string>& args);
    SsdCmdInterface* handleEraseCommand(const std::vector<std::string>& args);
    SsdCmdInterface* handleWriteCommand(const std::vector<std::string>& args);
    SsdCmdInterface* handleFlushCommand(const std::vector<std::string>& args);
    SsdCmdInterface* getReadCommandWithInput(const std::vector<std::string>& args);
    SsdCmdInterface* getEraseCommandWithInput(const std::vector<std::string>& args);
    SsdCmdInterface* getWriteCommandWithInput(const std::vector<std::string>& args);
    bool isHexString(const std::string& address);
    bool isLbaString(const std::string& address);
};