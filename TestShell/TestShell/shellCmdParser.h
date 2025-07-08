#pragma once
#include <cstdlib>  // for std::stol
#include <stdexcept>
#include <string>
#include <vector>

class TestShellCmdInterface;

class ShellCmdParser {
 public:
  TestShellCmdInterface* getCommand(const std::vector<std::string>& args);

 private:
  const int NUM_OF_READ_ARGS = 2;
  const int NUM_OF_WRITE_ARGS = 3;

  bool isHexString(const std::string& address);
  bool isRightLba(const std::string& address);
};