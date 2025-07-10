#include "shellCmdParser.h"
#include "shellCmd.h"


std::shared_ptr<shellCmdInterface> ShellCmdParser::getCommand(const std::vector<std::string>& args) {
	if (args.empty())             return makeErrorCommand();
	if (isReadCmd(args))          return handleReadCommand(args);
	if (isWriteCmd(args))         return handleWriteCommand(args);
	if (isHelpCmd(args))          return handleHelpCommand();
	if (isFullWriteCmd(args))     return handleFullWriteCommand(args);
	if (isFullReadCmd(args))      return handleFullReadCommand();
	if (isTestScript1(args))      return handleScript1Command();
	if (isTestScript2(args))      return handleScript2Command();
	if (isTestScript3(args))      return handleScript3Command();
	if (isTestScript4(args))      return handleScript4Command();
	if (isEraseCmd(args))         return handleEraseCommand(args);
	if (isEraseRangeCmd(args))    return handleEraseRangeCommand(args);
	if (isFlushCmd(args))         return handleFlushCommand();
	if (isExitCmd(args))          return handleExitCommand();
	return makeErrorCommand();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleReadCommand(const std::vector<std::string>& args) {
	long address = std::stol(args[1]);
	return std::make_shared<ShellReadCmd>(address);
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleWriteCommand(const std::vector<std::string>& args) {
	long address = std::stol(args[1]);
	unsigned value = std::stoul(args[2], nullptr, 16);
	return std::make_shared<ShellWriteCmd>(address, value);
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleHelpCommand() {
	return std::make_shared<ShellHelpCmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleFullWriteCommand(const std::vector<std::string>& args) {
	unsigned value = std::stoul(args[1], nullptr, 16);
	return std::make_shared<ShellFullWriteCmd>(value);
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleFullReadCommand() {
	return std::make_shared<ShellFullReadCmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleScript1Command() {
	return std::make_shared<ShellScript1Cmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleScript2Command() {
	return std::make_shared<ShellScript2Cmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleScript3Command() {
	return std::make_shared<ShellScript3Cmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleScript4Command() {
	return std::make_shared<ShellScript4Cmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleEraseCommand(const std::vector<std::string>& args) {
	long address = std::stol(args[1]);
	long size = std::stol(args[2]);
	return std::make_shared<ShellEraseCmd>(address, size);
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleEraseRangeCommand(const std::vector<std::string>& args) {
	long startAddress = std::stol(args[1]);
	long endAddress = std::stol(args[2]);
	return std::make_shared<ShellEraseRangeCmd>(startAddress, endAddress);
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleFlushCommand() {
	return std::make_shared<ShellFlushCmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::makeErrorCommand() {
	return std::make_shared<ShellErrorCmd>();
}

std::shared_ptr<shellCmdInterface> ShellCmdParser::handleExitCommand() {
	return nullptr;
}

bool ShellCmdParser::isReadCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_READ_ARGS) return false;
	if (args[0] != CMD_READ) return false;
	if (false == isLbaString(args[1])) return false;
	return true;
}

bool ShellCmdParser::isWriteCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_WRITE_ARGS) return false;
	if (args[0] != CMD_WRITE) return false;
	if (false == isLbaString(args[1])) return false;
	if (false == isHexString(args[2])) return false;
	return true;
}

bool ShellCmdParser::isExitCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	if (args[0] != CMD_EXIT) return false;
	return true;
}

bool ShellCmdParser::isHelpCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	if (args[0] != CMD_HELP) return false;
	return true;
}

bool ShellCmdParser::isFullWriteCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_FULL_WRITE_ARGS) return false;
	if (args[0] != CMD_FULL_WRITE) return false;
	if (false == isHexString(args[1])) return false;
	return true;
}

bool ShellCmdParser::isFullReadCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	if (args[0] != CMD_FULL_READ) return false;
	return true;
}

bool ShellCmdParser::isTestScript1(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	return (args[0] == CMD_SCRIPT_1 || args[0] == CMD_SCRIPT_SHORT_1);
}

bool ShellCmdParser::isTestScript2(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	return (args[0] == CMD_SCRIPT_2 || args[0] == CMD_SCRIPT_SHORT_2);
}

bool ShellCmdParser::isTestScript3(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	return (args[0] == CMD_SCRIPT_3 || args[0] == CMD_SCRIPT_SHORT_3);
}

bool ShellCmdParser::isEraseCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_ERASE_ARGS) return false;
	if (args[0] != CMD_ERASE) return false;
	if (false == isLbaString(args[1])) return false;
	if (false == isLbaString(args[2])) return false;
	return true;
}

bool ShellCmdParser::isEraseRangeCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_ERASE_ARGS) return false;
	if (args[0] != CMD_ERASE_RANGE) return false;
	if (false == isLbaString(args[1])) return false;
	if (false == isLbaString(args[2])) return false;
	return true;
}

bool ShellCmdParser::isFlushCmd(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	if (args[0] != CMD_FLUSH) return false;
	return true;
}

bool ShellCmdParser::isTestScript4(const std::vector<std::string>& args) {
	if (args.size() != NUM_OF_CMD_ONLY_ARGS) return false;
	return (args[0] == CMD_SCRIPT_4 || args[0] == CMD_SCRIPT_SHORT_4);
}

bool ShellCmdParser::isHexString(const std::string& address) {
	try {
		unsigned hexValue = std::stoul(address, nullptr, 16);
		return (address[0] == '0' && address[1] == 'x');
	}
	catch (...) {
	}
	return false;
}

bool ShellCmdParser::isLbaString(const std::string& address) {
	try {
		size_t pos;
		std::stoul(address, &pos);
		return pos == address.size();
	}
	catch (...) {
		return false;
	}
}