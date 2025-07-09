#include "testShellCmd.h"
#include "common.h"

TestShellErrorCmd::TestShellErrorCmd() {
}

bool TestShellErrorCmd::run() {
	std::cout << "unknown command \n";
	return false;
}