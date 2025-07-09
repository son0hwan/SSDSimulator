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

bool ShellReadCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	unsigned int value;

	if (executor->readFromSSDWithResult(address, &value)) {
		std::cout << "[Read] ERROR";
		return false;
	}

	printReadInfo(address, value);
	return true;
}

long ShellReadCmd::getAddress() { 
	return address; 
}

ShellFullReadCmd::ShellFullReadCmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellFullReadCmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::cout << "[Full Read] \n";
		
	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		unsigned int value;

		if (executor->readFromSSDWithResult(addr, &value)) {
			std::cout << "[Read] ERROR";
			return false;
		}

		printReadInfo(addr, value);
	}
	return true;
}