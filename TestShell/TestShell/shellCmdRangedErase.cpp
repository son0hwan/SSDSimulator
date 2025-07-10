#include "shellCmd.h"
#include "common.h"

ShellEraseRangeCmd::ShellEraseRangeCmd(long startAddress, long endAddress)
	: startAddress(startAddress), endAddress(endAddress) {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "RangedErase";
}

bool ShellEraseRangeCmd::run() {
	std::string result;

	int start_lba = startAddress;
	int exit_lba = endAddress + 1;
	int size = exit_lba - startAddress;

	while (start_lba != exit_lba) {
		int erase_size = size > ERASE_UNIT ? ERASE_UNIT : size;
		result = executor->eraseToSSD(start_lba, erase_size);
		if (result == ERROR_STRING) {
			printError();
			return false;
		}
		start_lba += erase_size;
		size -= erase_size;
	}
	printSuccess();
	return true;
}

long ShellEraseRangeCmd::getStartAddress() { return startAddress; }

long ShellEraseRangeCmd::getEndAddress() { return endAddress; }