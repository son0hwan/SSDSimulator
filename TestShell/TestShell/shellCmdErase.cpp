#include "shellCmd.h"
#include "common.h"

ShellEraseCmd::ShellEraseCmd(long address, long size) : address(address), size(size) {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Erase";
}

bool ShellEraseCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	int start_lba = address;
	int exit_lba = address + size;

	while (start_lba != exit_lba) {
		int erase_size = size > ERASE_UNIT ? ERASE_UNIT : size;
		if (result = executor->eraseToSSDWithResult(start_lba, erase_size))
			break;
		
		start_lba += erase_size;
	}
	printResult();
	return isCmdSuccess();
}

long ShellEraseCmd::getAddress() { return address; }

long ShellEraseCmd::getSize() { return size; }