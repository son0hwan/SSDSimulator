#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

#include "shellCmd.h"
#include "common.h"

static void printReadInfo(long address, unsigned int value)
{
	std::cout << "[Read] LBA "
		<< std::setw(2) << std::setfill('0') << std::dec << address
		<< " : 0x"
		<< std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value
		<< std::endl;
}

ShellReadCmd::ShellReadCmd(long address) : address{ address } {
	LOG(std::string(__FUNCTION__) + " has been called");
}

void ShellReadCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	executor->readFromSSD(address);

	std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
	if (hexStr._Equal("ERROR")) {
		std::cout << "[Read] ERROR\n";
		return;
	}

	unsigned int value = std::stoul(hexStr, nullptr, 16);
	printReadInfo(address, value);
}

long ShellReadCmd::getAddress() { 
	return address; 
}

ShellFullReadCmd::ShellFullReadCmd() {}

void ShellFullReadCmd::run() {
	std::cout << "[Full Read] \n";
		
	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		if (executor->readFromSSD(addr) == ERROR_STRING)
			return;

		std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
		if (hexStr._Equal("ERROR")) {
			std::cout << "[Read] ERROR";
			return;
		}

		unsigned int value = std::stoul(hexStr, nullptr, 16);
		printReadInfo(addr, value);
	}
}