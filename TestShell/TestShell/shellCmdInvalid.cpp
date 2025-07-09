#include "shellCmd.h"
#include "common.h"

ShellInvalidCmd::ShellInvalidCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

	bool ShellInvalidCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");
	std::cout << "invalid command \n";
	return false;
}