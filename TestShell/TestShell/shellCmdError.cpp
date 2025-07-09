#include "shellCmd.h"
#include "common.h"

ShellErrorCmd::ShellErrorCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

void ShellErrorCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");
	std::cout << "unknown command \n";
}