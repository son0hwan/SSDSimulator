#include "ssdCmdParser.h"

#include <algorithm>

#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdInterface.h"

bool SsdCmdParser::isHexString(const std::string& address) {
  try {
    long hexValue = std::stol(address, nullptr, 16);
    return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
  } catch (...) {
    return false;
  }
}

bool SsdCmdParser::isRightLba(const std::string& address) {
  try {
    long lba = std::stol(address);
    return 0 <= lba && lba <= 99;
  } catch (...) {
    return false;
  }
}

SsdCmdInterface* SsdCmdParser::getCommand(
    const std::vector<std::string>& args) {
  if (args.empty()) return new SsdErrorCmd(-1);

  std::string cmd = args[0];

  if (isReadCmd(args)) {
    long address = std::stol(args[1]);
    return new SsdReadCmd(address);
  } else if (isWriteCmd(args)) {
    long address = std::stol(args[1]);
    long value = std::stol(args[2], nullptr, 16);  // hex format
    return new SsdWriteCmd(address, value);
  } else {
    return new SsdErrorCmd(-1);  // -1 is a placeholder for error command
  }
  return nullptr;
}

bool SsdCmdParser::isReadCmd(const std::vector<std::string>& args) {
  if (args.size() != NUM_OF_READ_ARGS) return false;
  if (args[0] != "R") return false;
  if (false == isRightLba(args[1])) return false;
  return true;
}

bool SsdCmdParser::isWriteCmd(const std::vector<std::string>& args) {
  if (args.size() != NUM_OF_WRITE_ARGS) return false;
  if (args[0] != "W") return false;
  if (false == isRightLba(args[1])) return false;
  if (false == isHexString(args[2])) return false;
  return true;
}