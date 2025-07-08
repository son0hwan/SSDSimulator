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

  const int MIN_ADDRESS = 0;
  const int MAX_ADDRESS = 99;

  static constexpr const char* CMD_READ = "read";
  static constexpr const char* CMD_WRITE = "write";

  bool isHexString(const std::string& address);
  bool isRightLba(const std::string& address);

  bool isReadCmd(const std::vector<std::string>& args);
  bool isWriteCmd(const std::vector<std::string>& args);
};