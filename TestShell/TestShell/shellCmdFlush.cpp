#include "shellCmd.h"
#include "common.h"

ShellFlushCmd::ShellFlushCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Flush";
}

bool ShellFlushCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	result = executor->flushToSSD();
	if (result == ERROR_STRING) {
		std::cout << "[Flush] ERROR" << std::endl << std::endl;
		return false;
	}
	else if (result == FAIL) {
		std::cout << "[Flush] FAIL to execute ssd.exe" << std::endl << std::endl;
		return false;
	}

	std::cout << "[Flush] Done" << std::endl << std::endl;

	return true;
}