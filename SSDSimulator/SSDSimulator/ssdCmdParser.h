#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>  // for std::stol
#include "ssdInterface.h"

class SsdCmdParser {
public:
    std::shared_ptr<SsdCmdInterface> getCommand(const std::vector<std::string>& args);
    std::vector<std::string> getEncodedString(SsdCmdInterface* cmd);

private:
    const int NUM_OF_READ_ARGS = 2;
    const int NUM_OF_WRITE_ARGS = 3;
    const int NUM_OF_ERASE_ARGS = 3;
    const int NUM_OF_FLUSH_ARGS = 1;

    std::shared_ptr<SsdCmdInterface> handleReadCommand(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> handleEraseCommand(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> handleWriteCommand(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> handleFlushCommand(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> getReadCommandWithInput(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> getEraseCommandWithInput(const std::vector<std::string>& args);
    std::shared_ptr<SsdCmdInterface> getWriteCommandWithInput(const std::vector<std::string>& args);
    bool isHexString(const std::string& address);
    bool isLbaString(const std::string& address);

    std::vector<std::string> getWriteCommandString(SsdCmdInterface* cmd);
    std::vector<std::string> getEraseCommandString(SsdCmdInterface* cmd);
};