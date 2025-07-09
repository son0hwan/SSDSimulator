#include "shellCmdParser.h"
#include "shellCmd.h"

shellCmdInterface* ShellCmdParser::getCommand(
	const std::vector<std::string>& args) {
	if (args.empty()) return new ShellErrorCmd{};

	if (isReadCmd(args)) {
		long address = std::stol(args[1]);
		return new ShellReadCmd{ address };
	}
	else if (isWriteCmd(args)) {
		long address = std::stol(args[1]);
		unsigned value = std::stoul(args[2], nullptr, 16);
		return new ShellWriteCmd{ address, value };
	}
	else if (isHelpCmd(args)) {
		return new ShellHelpCmd{};
	}
	else if (isFullWriteCmd(args)) {
		unsigned value = std::stoul(args[1], nullptr, 16);
		return new ShellFullWriteCmd{ value };
	}
	else if (isFullReadCmd(args)) {
		return new ShellFullReadCmd{};
	}
	else if (isExitCmd(args)) {
		return SHELL_EXIT_CMD;
	}
	else if (isTestScript1(args)) {
		return new ShellScript1Cmd{};
	}
	else if (isTestScript2(args)) {
		return new ShellScript2Cmd{};
	}
	else if (isTestScript3(args)) {
		return new ShellScript3Cmd{};
	}
	else if (isEraseCmd(args)) {
		long address = std::stol(args[1]);
		long size = std::stol(args[2]);
		return new ShellEraseCmd{ address, size };
	}
	else if (isEraseRangeCmd(args)) {
		long startAddress = std::stol(args[1]);
		long endAddress = std::stol(args[2]);
		return new ShellEraseRangeCmd{ startAddress, endAddress };
	}
	else if (isFlushCmd(args)) {
		return new ShellFlushCmd{};
	}
	else if (isTestScript4(args)) {
		return new ShellScript4Cmd{};
	}

	return new ShellErrorCmd();
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