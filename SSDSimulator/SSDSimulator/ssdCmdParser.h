#pragma once

#include <string>
#include <stdexcept>
#include <cstdlib>  // for std::stol

class SsdCmdParser {
public:
    SsdCmdInterface* parse(const std::string& cmd, const std::string& arg1, const std::string& arg2 = "");
};