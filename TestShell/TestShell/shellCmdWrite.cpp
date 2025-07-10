#pragma once

#include "shellCmd.h"
#include "common.h"

ShellWriteCmd::ShellWriteCmd(long address, unsigned data)
	: address(address), data(data) {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Write";
}

bool ShellWriteCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	if (executor->writeToSSDWithResult(address, data)) {
		printError();
		return false;
	}

	std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
	if (hexStr._Equal(ERROR_STRING)) {
		printError();
		return false;
	}

	printSuccess();
	return true;
}

long ShellWriteCmd::getAddress() { 
	return address; 
}

long ShellWriteCmd::getData() {
	return data; 
}

ShellFullWriteCmd::ShellFullWriteCmd(unsigned data)
	: data(data) {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "Full Write";
}

bool ShellFullWriteCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		if (executor->writeToSSDWithResult(addr, data)) {
			printError();
			return false;
		}
	}

	printSuccess();
	return true;
}