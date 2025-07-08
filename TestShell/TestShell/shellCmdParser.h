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
  const int NUM_OF_CMD_ONLY_ARGS = 1;

  const int MIN_ADDRESS = 0;
  const int MAX_ADDRESS = 99;

  static constexpr const char* CMD_READ = "read";
  static constexpr const char* CMD_WRITE = "write";
  static constexpr const char* CMD_EXIT = "exit";
  static constexpr const char* CMD_HELP = "help";

  bool isHexString(const std::string& address);
  bool isLbaString(const std::string& address);

  bool isReadCmd(const std::vector<std::string>& args);
  bool isWriteCmd(const std::vector<std::string>& args);
  bool isExitCmd(const std::vector<std::string>& args);
  bool isHelpCmd(const std::vector<std::string>& args);
};