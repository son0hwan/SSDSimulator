#include "testShellCmd.h"
#include "common.h"

TestShellErrorCmd::TestShellErrorCmd() {
}

void TestShellErrorCmd::run() {
	std::cout << "unknown command \n";
}