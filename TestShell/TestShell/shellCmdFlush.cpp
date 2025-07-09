#include "shellCmd.h"
#include "common.h"

ShellFlushCmd::ShellFlushCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellFlushCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");
	return true;
}