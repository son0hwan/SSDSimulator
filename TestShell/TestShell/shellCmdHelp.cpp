#include "testShellCmd.h"
#include "common.h"

TestShellHelpCmd::TestShellHelpCmd() {}

void TestShellHelpCmd::run() {
	std::cout << "\n [HELP]\n";
	std::cout << " -. read [LBA] \n";
	std::cout << " -. write [LBA] [DATA_IN_HEX] \n";
	std::cout << " -. fullread \n";
	std::cout << " -. fullwrite [DATA_IN_HEX] \n";
	std::cout << " -. exit \n\n";
}