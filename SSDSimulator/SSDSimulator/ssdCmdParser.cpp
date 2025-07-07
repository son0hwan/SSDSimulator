#include "ssdInterface.h"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "ssdCmdParser.h"	

#include <algorithm>

bool SsdCmdParser::isHexString(const std::string& address) {
	return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
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
	std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

	if (((cmd == "r" && args.size() == NUM_OF_READ_ARGS) || (cmd == "read" && args.size() == NUM_OF_READ_ARGS))
		&& isRightLba(args[1])) {
		long address = std::stol(args[1]);
		return new SsdReadCmd(address);
	}
	else if (((cmd == "w" && args.size() == NUM_OF_WRITE_ARGS) || (cmd == "write" && args.size() == NUM_OF_WRITE_ARGS))
		&& (isRightLba(args[1]) && isHexString(args[2]))) {
		long address = std::stol(args[1]);
		long value = std::stol(args[2], nullptr, 16); // hex format
		return new SsdWriteCmd(address, value);
	}
	else {
		return new SsdErrorCmd(-1); // -1 is a placeholder for error command
	}
	return nullptr;
}
