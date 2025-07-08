#include "shellCmdParser.h"
#include "testShellCmdRead.cpp"
#include "testShellCmdWrite.cpp"

bool ShellCmdParser::isHexString(const std::string& address) {
  try {
    unsigned hexValue = std::stoul(address, nullptr, 16);
    return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
  } catch (...) {
    return false;
  }
}

bool ShellCmdParser::isRightLba(const std::string& address) {
  try {
    long lba = std::stol(address);
    return 0 <= lba && lba <= 99;
  } catch (...) {
    return false;
  }
}

TestShellCmdInterface* ShellCmdParser::getCommand(
    const std::vector<std::string>& args) {
  if (args.empty()) return new TestShellErrorCmd{};

  std::string cmd = args[0];

  if ((cmd == "read" && args.size() == NUM_OF_READ_ARGS) && isRightLba(args[1])) {
    long address = std::stol(args[1]);
    return new TestShellReadCmd(address);
  } else if ((cmd == "write" && args.size() == NUM_OF_WRITE_ARGS) &&
             (isRightLba(args[1]) && isHexString(args[2]))) {
    long address = std::stol(args[1]);
    unsigned value = std::stoul(args[2], nullptr, 16);  // hex format
    return new TestShellWriteCmd(address, value);
  } else {
    return new TestShellErrorCmd();  // -1 is a placeholder for error command
  }
  return nullptr;
}
