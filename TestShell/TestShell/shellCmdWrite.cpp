#pragma once

#include "shellCmd.h"
#include "common.h"

ShellWriteCmd::ShellWriteCmd(long address, unsigned data)
	: address(address), data(data) {
	LOG(std::string(__FUNCTION__) + " has been called");
}

void ShellWriteCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	result = executor->writeToSSD(address, data);
	if (result == ERROR_STRING) {
		std::cout << "[Write] ERROR" << std::endl;
		return;
	}
	std::cout << "[Write] Done" << std::endl;
}

long ShellWriteCmd::getAddress() { 
	return address; 
}

long ShellWriteCmd::getData() {
	return data; 
}

ShellFullWriteCmd::ShellFullWriteCmd(unsigned data)
	: data(data) {}

void ShellFullWriteCmd::run() {
	std::string result;

	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		result = executor->writeToSSD(addr, data);
		if (result == ERROR_STRING) {
			std::cout << "[FullWrite] Error" << std::endl;
			return;
		}
	}

	std::cout << "[FullWrite] Done" << std::endl;
}