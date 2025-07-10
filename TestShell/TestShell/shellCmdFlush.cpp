#include "shellCmd.h"
#include "common.h"

ShellFlushCmd::ShellFlushCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Flush";
}

bool ShellFlushCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");
	return true;
}