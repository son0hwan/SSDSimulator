#include "shellCmd.h"
#include "common.h"

ShellEraseCmd::ShellEraseCmd(long address, long size) : address(address), size(size) {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellEraseCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	int start_lba = address;
	int exit_lba = address + size;

	while (start_lba != exit_lba) {
		int erase_size = size > ERASE_UNIT ? ERASE_UNIT : size;
		result = executor->eraseToSSD(start_lba, erase_size);
		if (result == ERROR_STRING) {
			std::cout << "[Erase] ERROR" << std::endl;
			return false;
		}
		start_lba += erase_size;
	}
	std::cout << "[Erase] Done" << std::endl;
	return true;
}

long ShellEraseCmd::getAddress() { return address; }

long ShellEraseCmd::getSize() { return size; }