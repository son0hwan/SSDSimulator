#include "shellCmd.h"
#include "common.h"

ShellEraseRangeCmd::ShellEraseRangeCmd(long startAddress, long endAddress)
	: startAddress(startAddress), endAddress(endAddress) {
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
			std::cout << "[RangedErase] ERROR" << std::endl << std::endl;
			return false;
		}
		start_lba += erase_size;
		size -= erase_size;
	}
	std::cout << "[RangedErase] Done" << std::endl << std::endl;
	return true;
}

long ShellEraseRangeCmd::getStartAddress() { return startAddress; }

long ShellEraseRangeCmd::getEndAddress() { return endAddress; }