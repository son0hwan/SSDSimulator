#include "testShellCmd.h"
#include "common.h"

TestShellInvalidCmd::TestShellInvalidCmd() {
}

bool TestShellInvalidCmd::run() {
	std::cout << "invalid command \n";
	return false;
}