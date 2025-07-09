#pragma once

#include "testShellCmd.h"
#include "common.h"

TestShellWriteCmd::TestShellWriteCmd(long address, unsigned data)
	: address(address), data(data) {
}

void TestShellWriteCmd::run() {
	if (executor->writeToSSDWithResult(address, data)) {
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
	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		if (executor->writeToSSDWithResult(addr, data)) {
			std::cout << "[FullWrite] Error" << std::endl;
			return;
		}
	}
	std::cout << "[FullWrite] Done" << std::endl;
}