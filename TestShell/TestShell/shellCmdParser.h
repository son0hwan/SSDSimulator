#pragma once
#include <cstdlib>  // for std::stol
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>

class shellCmdInterface;

class ShellCmdParser {
public:
	std::shared_ptr<shellCmdInterface> getCommand(const std::vector<std::string>& args);

private:
	static const int NUM_OF_READ_ARGS = 2;
	static const int NUM_OF_WRITE_ARGS = 3;
	static const int NUM_OF_CMD_ONLY_ARGS = 1;
	static const int NUM_OF_FULL_WRITE_ARGS = 2;
	static const int NUM_OF_ERASE_ARGS = 3;

	static const int MIN_ADDRESS = 0;
	static const int MAX_ADDRESS = 99;

	static constexpr const char* CMD_READ = "read";
	static constexpr const char* CMD_WRITE = "write";
	static constexpr const char* CMD_EXIT = "exit";
	static constexpr const char* CMD_HELP = "help";
	static constexpr const char* CMD_FULL_WRITE = "fullwrite";
	static constexpr const char* CMD_FULL_READ = "fullread";
	static constexpr const char* CMD_ERASE = "erase";
	static constexpr const char* CMD_ERASE_RANGE = "erase_range";
	static constexpr const char* CMD_FLUSH = "flush";
	static constexpr const char* CMD_SCRIPT_1 = "1_FullWriteAndReadCompare";
	static constexpr const char* CMD_SCRIPT_2 = "2_PartialLBAWrite";
	static constexpr const char* CMD_SCRIPT_3 = "3_WriteReadAging";
	static constexpr const char* CMD_SCRIPT_4 = "4_EraseAndWriteAging";
	static constexpr const char* CMD_SCRIPT_SHORT_1 = "1_";
	static constexpr const char* CMD_SCRIPT_SHORT_2 = "2_";
	static constexpr const char* CMD_SCRIPT_SHORT_3 = "3_";
	static constexpr const char* CMD_SCRIPT_SHORT_4 = "4_";

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

	bool isEraseCmd(const std::vector<std::string>& args);
	bool isEraseRangeCmd(const std::vector<std::string>& args);
	bool isFlushCmd(const std::vector<std::string>& args);
	bool isTestScript4(const std::vector<std::string>& args);

	std::shared_ptr<shellCmdInterface> handleReadCommand(const std::vector<std::string>& args);
	std::shared_ptr<shellCmdInterface> handleWriteCommand(const std::vector<std::string>& args);
	std::shared_ptr<shellCmdInterface> handleEraseCommand(const std::vector<std::string>& args);
	std::shared_ptr<shellCmdInterface> handleEraseRangeCommand(const std::vector<std::string>& args);
	std::shared_ptr<shellCmdInterface> handleFullWriteCommand(const std::vector<std::string>& args);
	std::shared_ptr<shellCmdInterface> handleHelpCommand();
	std::shared_ptr<shellCmdInterface> handleFullReadCommand();
	std::shared_ptr<shellCmdInterface> handleScript1Command();
	std::shared_ptr<shellCmdInterface> handleScript2Command();
	std::shared_ptr<shellCmdInterface> handleScript3Command();
	std::shared_ptr<shellCmdInterface> handleScript4Command();
	std::shared_ptr<shellCmdInterface> handleFlushCommand();
	std::shared_ptr<shellCmdInterface> makeErrorCommand();
	std::shared_ptr<shellCmdInterface> handleExitCommand();
};