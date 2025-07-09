#include "ssdCmdIncludes.h"
#include <string>
#include <algorithm>
#include <cstdint>


SsdCmdInterface* SsdCmdParser::getCommand(const std::vector<std::string>& args) {
	if (args.empty()) return new SsdErrorCmd();

	std::string cmd = args[0];
	if (cmd == "R") return handleReadCommand(args);
	if (cmd == "W") return handleWriteCommand(args);
	if (cmd == "E") return handleEraseCommand(args);
	if (cmd == "F") return handleFlushCommand(args);

	return new SsdErrorCmd();
}

SsdCmdInterface* SsdCmdParser::handleReadCommand(const std::vector<std::string>& args)
{
	if (!(args.size() == NUM_OF_READ_ARGS)) return new SsdErrorCmd();
	if (!isLbaString(args[1])) return new SsdErrorCmd();
	return getReadCommandWithInput(args);
}

bool SsdCmdParser::isLbaString(const std::string& address) {
	try {
		size_t pos;
		std::stoul(address, &pos);
		return pos == address.size();
	}
	catch (...) {
		return false;
	}
}

SsdCmdInterface* SsdCmdParser::getReadCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stol(args[1], nullptr, 10);

	SsdReadCmd* readCmd = new SsdReadCmd(address);
	return readCmd;
}

SsdCmdInterface* SsdCmdParser::handleWriteCommand(const std::vector<std::string>& args) {
	if (!(args.size() == NUM_OF_WRITE_ARGS)) return new SsdErrorCmd();
	if (!isLbaString(args[1])) return new SsdErrorCmd();
	if (!isHexString(args[2])) return new SsdErrorCmd();
	return getWriteCommandWithInput(args);
}

SsdCmdInterface* SsdCmdParser::handleEraseCommand(const std::vector<std::string>& args) {
	if (!(args.size() == NUM_OF_ERASE_ARGS)) return new SsdErrorCmd();
	if (!isLbaString(args[1])) return new SsdErrorCmd();
	if (!isLbaString(args[2])) return new SsdErrorCmd();
	return getEraseCommandWithInput(args);
}

SsdCmdInterface* SsdCmdParser::getEraseCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stoul(args[1], nullptr, 10);
	uint32_t size = std::stoul(args[2], nullptr, 10);

	SsdEraseCmd* eraseCmd = new SsdEraseCmd(address, size);
	return eraseCmd;
}

bool SsdCmdParser::isHexString(const std::string& address) {
	try {
		long hexValue = std::stol(address, nullptr, 16);
		return (address.length() > 2 && (address[0] == '0' && address[1] == 'x'));
	}
	catch (...) {
		return false;
	}
}

SsdCmdInterface* SsdCmdParser::getWriteCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stoul(args[1], nullptr, 10);
	uint32_t value = std::stoul(args[2], nullptr, 16);

	SsdWriteCmd* writeCmd = new SsdWriteCmd(address, value);
	return writeCmd;
}

SsdCmdInterface* SsdCmdParser::handleFlushCommand(const std::vector<std::string>& args)
{
	if (!(args.size() == NUM_OF_FLUSH_ARGS)) return new SsdErrorCmd();
	return new SsdFlushCmd();
}
