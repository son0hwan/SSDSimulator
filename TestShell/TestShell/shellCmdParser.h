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
  const int NUM_OF_FULL_WRITE_ARGS = 2;

  const int MIN_ADDRESS = 0;
  const int MAX_ADDRESS = 99;

  static constexpr const char* CMD_READ = "read";
  static constexpr const char* CMD_WRITE = "write";
  static constexpr const char* CMD_EXIT = "exit";
  static constexpr const char* CMD_HELP = "help";
  static constexpr const char* CMD_FULL_WRITE = "fullwrite";
  static constexpr const char* CMD_FULL_READ = "fullread";
  static constexpr const char* CMD_SCRIPT_1 = "1_FullWriteAndReadCompare";
  static constexpr const char* CMD_SCRIPT_2 = "2_PartialLBAWrite";
  static constexpr const char* CMD_SCRIPT_3 = "3_WriteReadAging";
  static constexpr const char* CMD_SCRIPT_SHORT_1 = "1_";
  static constexpr const char* CMD_SCRIPT_SHORT_2 = "2_";
  static constexpr const char* CMD_SCRIPT_SHORT_3 = "3_";

  bool isHexString(const std::string& address);
  bool isLbaString(const std::string& address);

  bool isReadCmd(const std::vector<std::string>& args);
  bool isWriteCmd(const std::vector<std::string>& args);
  bool isExitCmd(const std::vector<std::string>& args);
  bool isHelpCmd(const std::vector<std::string>& args);
  bool isFullWriteCmd(const std::vector<std::string>& args);
  bool isFullReadCmd(const std::vector<std::string>& args);
  bool isTestScript1(const std::vector<std::string>& args);
  bool isTestScript2(const std::vector<std::string>& args);
  bool isTestScript3(const std::vector<std::string>& args);
};