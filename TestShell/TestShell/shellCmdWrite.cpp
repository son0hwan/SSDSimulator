#pragma once

#include "testShellCmd.h"
#include "common.h"

TestShellWriteCmd::TestShellWriteCmd(long address, unsigned data)
	: address(address), data(data) {
}

void TestShellWriteCmd::run() {
	std::string result;

	result = executor->writeToSSD(address, data);
	if (result == ERROR_STRING) {
		std::cout << "[Write] Error" << std::endl;
		return;
	}
	std::cout << "[Write] Done" << std::endl;
}

long TestShellWriteCmd::getAddress() { 
	return address; 
}

long TestShellWriteCmd::getData() {
	return data; 
}

TestShellFullWriteCmd::TestShellFullWriteCmd(unsigned data)
	: data(data) {}

void TestShellFullWriteCmd::run() {
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