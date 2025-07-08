#include "ssdInterface.h"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdCmdError.h"
#include "ssdCmdParser.h"	
#include <string>
#include <algorithm>
#include <cstdint>

bool SsdCmdParser::isHexString(const std::string& address) {
	try {
		long hexValue = std::stol(address, nullptr, 16);
		return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
	}
	catch (...) {
		return false;
	}
}

bool SsdCmdParser::isLbaString(const std::string& address) {
	try {
		long lba = std::stol(address);
		return MIN_ADDRESS <= lba && lba <= MAX_ADDRESS;
	}
	catch (...) {
		return false;
	}
}

SsdCmdInterface* SsdCmdParser::getCommand(const std::vector<std::string>& args) {
	if (args.empty()) return new SsdErrorCmd(INVALID_ADDRESS);

	std::string cmd = args[0];

	if ((cmd == "R" && args.size() == NUM_OF_READ_ARGS) && isLbaString(args[1])) {
		uint32_t address = std::stol(args[1]);
		SsdReadCmd& readCmd = SsdReadCmd::getInstance();
		readCmd.setAddress(address);
		return &SsdReadCmd::getInstance();
	}
	else if ((cmd == "W" && args.size() == NUM_OF_WRITE_ARGS) && (isLbaString(args[1]) && isHexString(args[2]))) {
		uint32_t address = std::stol(args[1]);
		uint32_t value = std::stol(args[2], nullptr, 16); // hex format		
		SsdWriteCmd& writeCmd = SsdWriteCmd::getInstance();
		writeCmd.setAddress(address);
		writeCmd.setWriteData(value);
		return &SsdWriteCmd::getInstance();
	}
	else {
		return new SsdErrorCmd(-1); // -1 is a placeholder for error command
	}
	return nullptr;
}
