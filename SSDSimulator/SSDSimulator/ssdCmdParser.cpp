#include "ssdInterface.h"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdCmdParser.h"	
#include<string>

#include <algorithm>

bool SsdCmdParser::isHexString(const std::string& address) {
	try {
		long hexValue = std::stol(address, nullptr, 16);
		return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
	}
	catch (...) {
		return false;
	}
}

bool SsdCmdParser::isRightLba(const std::string& address) {
	try {
		long lba = std::stol(address);
		return 0 <= lba && lba <= 99;
	}
	catch (...) {
		return false;
	}
}

SsdCmdInterface* SsdCmdParser::getCommand(const std::vector<std::string>& args) {
	if (args.empty()) return new SsdErrorCmd(-1);

	std::string cmd = args[0];

	if ((cmd == "R" && args.size() == NUM_OF_READ_ARGS) && isRightLba(args[1])) {
		long address = std::stol(args[1]);
		SsdReadCmd& readCmd = SsdReadCmd::getInstance();
		long address = std::stol(arg1);
		readCmd.setAddress(address);
		return &SsdReadCmd::getInstance();
	}
	else if ((cmd == "W" && args.size() == NUM_OF_WRITE_ARGS) && (isRightLba(args[1]) && isHexString(args[2]))) {
		long address = std::stol(args[1]);
		long value = std::stol(args[2], nullptr, 16); // hex format
		return new SsdWriteCmd(address, value);
	}
	else {
		return new SsdErrorCmd(-1); // -1 is a placeholder for error command
	}
	return nullptr;
}