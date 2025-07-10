#include "shellCmd.h"
#include "common.h"

ShellHelpCmd::ShellHelpCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Help";
}

bool ShellHelpCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::cout << "\n [HELP]\n";
	std::cout << " -. read [LBA] \n";
	std::cout << " -. write [LBA] [DATA_IN_HEX] \n";
	std::cout << " -. fullread \n";
	std::cout << " -. fullwrite [DATA_IN_HEX] \n";
	std::cout << " -. erase [LBA] [SIZE] \n";
	std::cout << " -. erase_range [START_LBA] [END_LBA] \n";
	std::cout << " -. exit \n\n";
	return isCmdSuccess();
}