#pragma once

#include "shellCmd.h"
#include "common.h"

ShellWriteCmd::ShellWriteCmd(long address, unsigned data)
	: address(address), data(data) {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellWriteCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	result = executor->writeToSSD(address, data);
	if (result == ERROR_STRING) {
		std::cout << "[Write] Error" << std::endl << std::endl;
		return false;
	}

	std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
	if (hexStr._Equal("ERROR")) {
		std::cout << "[Write] ERROR" << std::endl << std::endl;
		return false;
	}

	std::cout << "[Write] Done" << std::endl << std::endl;
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
}

bool ShellFullWriteCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		result = executor->writeToSSD(addr, data);
		
		if (result == ERROR_STRING) {
			std::cout << "[Full Write] Error" << std::endl<< std::endl;
			return false;
		}

		std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
		if (hexStr._Equal("ERROR")) {
			std::cout << "[Full Write] ERROR" << std::endl << std::endl;
			return false;
		}
	}

	std::cout << "[Full Write] Done" << std::endl << std::endl;
	return true;
}