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

void TestShellReadCmd::run() {
	unsigned int value;

	if (executor->readFromSSDWithResult(address, &value)) {
		std::cout << "[Read] ERROR";
		return;
	}

	printReadInfo(address, value);
}

long TestShellReadCmd::getAddress() { 
	return address; 
}

TestShellFullReadCmd::TestShellFullReadCmd() {}

void TestShellFullReadCmd::run() {
	std::cout << "[Full Read] \n";
		
	for (int addr = 0; addr < NUM_OF_LBA; addr++) {
		unsigned int value;

		if (executor->readFromSSDWithResult(addr, &value)) {
			std::cout << "[Read] ERROR";
			return;
		}

		printReadInfo(addr, value);
	}
}