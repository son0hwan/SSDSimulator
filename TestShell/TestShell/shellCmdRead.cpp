#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>

#include "testShellCmd.h"
#include "common.h"

static void printReadInfo(long address, unsigned int value)
{
	std::cout << "[Read] LBA "
		<< std::setw(2) << std::setfill('0') << std::dec << address
		<< " : 0x"
		<< std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value
		<< std::endl;
}

TestShellReadCmd::TestShellReadCmd(long address) : address{ address } {}

bool TestShellReadCmd::run() {
	executor->readFromSSD(address);

	std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
	if (hexStr._Equal("ERROR")) {
		std::cout << "[Read] ERROR";
		return false;
	}

	unsigned int value = std::stoul(hexStr, nullptr, 16);
	printReadInfo(address, value);
	return true;
}

long TestShellReadCmd::getAddress() { 
	return address; 
}

TestShellFullReadCmd::TestShellFullReadCmd() {}

bool TestShellFullReadCmd::run() {
	std::cout << "[Full Read] \n";
		
	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		if (executor->readFromSSD(addr) == ERROR_STRING)
			return false;

		std::string hexStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
		if (hexStr._Equal("ERROR")) {
			std::cout << "[Read] ERROR";
			return false;
		}

		unsigned int value = std::stoul(hexStr, nullptr, 16);
		printReadInfo(addr, value);
	}
	return true;
}