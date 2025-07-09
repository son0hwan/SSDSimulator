#include "ssdCmdIncludes.h"
#include <string>
#include <algorithm>
#include <cstdint>
#include <sstream>


std::shared_ptr<SsdCmdInterface> SsdCmdParser::getCommand(const std::vector<std::string>& args) {
	if (args.empty()) return std::make_shared<SsdErrorCmd>();

	std::string cmd = args[0];
	if (cmd == "R") return handleReadCommand(args);
	if (cmd == "W") return handleWriteCommand(args);
	if (cmd == "E") return handleEraseCommand(args);
	if (cmd == "F") return handleFlushCommand(args);

	return std::make_shared<SsdErrorCmd>();
}

std::shared_ptr<SsdCmdInterface> SsdCmdParser::handleReadCommand(const std::vector<std::string>& args)
{
	if (!(args.size() == NUM_OF_READ_ARGS)) return std::make_shared<SsdErrorCmd>();
	if (!isLbaString(args[1])) return std::make_shared<SsdErrorCmd>();
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

std::shared_ptr<SsdCmdInterface> SsdCmdParser::getReadCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stol(args[1], nullptr, 10);
	return std::make_shared<SsdReadCmd>(address);
}

std::shared_ptr<SsdCmdInterface> SsdCmdParser::handleWriteCommand(const std::vector<std::string>& args) {
	if (!(args.size() == NUM_OF_WRITE_ARGS)) return std::make_shared<SsdErrorCmd>();
	if (!isLbaString(args[1])) return std::make_shared<SsdErrorCmd>();
	if (!isHexString(args[2])) return std::make_shared<SsdErrorCmd>();
	return getWriteCommandWithInput(args);
}

std::shared_ptr<SsdCmdInterface> SsdCmdParser::handleEraseCommand(const std::vector<std::string>& args) {
	if (!(args.size() == NUM_OF_ERASE_ARGS)) return std::make_shared<SsdErrorCmd>();
	if (!isLbaString(args[1])) return std::make_shared<SsdErrorCmd>();
	if (!isLbaString(args[2])) return std::make_shared<SsdErrorCmd>();
	return getEraseCommandWithInput(args);
}

std::shared_ptr<SsdCmdInterface> SsdCmdParser::getEraseCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stoul(args[1], nullptr, 10);
	uint32_t size = std::stoul(args[2], nullptr, 10);
	return std::make_shared<SsdEraseCmd>(address, size);
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

std::shared_ptr<SsdCmdInterface> SsdCmdParser::getWriteCommandWithInput(const std::vector<std::string>& args) {
	uint32_t address = std::stoul(args[1], nullptr, 10);
	uint32_t value = std::stoul(args[2], nullptr, 16);
	return std::make_shared<SsdWriteCmd>(address, value);
}

std::shared_ptr<SsdCmdInterface> SsdCmdParser::handleFlushCommand(const std::vector<std::string>& args)
{
	if (!(args.size() == NUM_OF_FLUSH_ARGS)) return std::make_shared<SsdErrorCmd>();
	return std::make_shared<SsdFlushCmd>();
}

std::vector<std::string> SsdCmdParser::getEncodedString(SsdCmdInterface* cmd) {
	if (nullptr != dynamic_cast<SsdWriteCmd*>(cmd)) return getWriteCommandString(cmd);
	if (nullptr != dynamic_cast<SsdEraseCmd*>(cmd)) return getEraseCommandString(cmd);
	return {};
}

std::vector<std::string> SsdCmdParser::getWriteCommandString(SsdCmdInterface* cmd) {
	std::vector<std::string> result;
	std::stringstream ss;
	auto writeCmd = dynamic_cast<SsdWriteCmd*>(cmd);

	result.push_back("W");
	result.push_back(std::to_string(writeCmd->getAddress()));

	ss << "0x"; 
	ss << std::uppercase << std::hex << std::setw(8) << std::setfill('0') << writeCmd->getData();

	result.push_back(ss.str());
	return result;
}

std::vector<std::string> SsdCmdParser::getEraseCommandString(SsdCmdInterface* cmd) {
	std::vector<std::string> result;
	auto eraseCmd = dynamic_cast<SsdEraseCmd*>(cmd);

	result.push_back("E");
	result.push_back(std::to_string(eraseCmd->getStartAddress()));
	result.push_back(std::to_string(eraseCmd->getSize()));
	return result;
}