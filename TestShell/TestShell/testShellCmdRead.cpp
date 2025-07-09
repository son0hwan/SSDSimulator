#pragma once

#include "testShellCmdInterface.h"
#include <iostream>
#include <fstream>
#include <iomanip>

static void printReadInfo(long address, unsigned int value)
{
	std::cout << "[Read] LBA "
		<< std::setw(2) << std::setfill('0') << std::dec << address
		<< " : 0x"
		<< std::setw(8) << std::setfill('0') << std::hex << std::uppercase << value
		<< std::endl;
}

static void readOutputFile(long address) {
	std::ifstream file("ssd_output.txt");

	if (!file.is_open()) {
		std::cerr << "fail to open file" << std::endl;
		return;
	}

	std::string line;
	if (std::getline(file, line)) {
		unsigned int value = std::stoul(line, nullptr, 16);
		printReadInfo(address, value);
	}
	else {
		std::cout << "fail to read file" << std::endl;
	}

	file.close();
}

class TestShellReadCmd : public TestShellCmdInterface {
public:
	TestShellReadCmd(long address) : address(address) {}

	void run() override {
		executor->readFromSSD(address);
		readOutputFile(address);
	}

	long getAddress() { return address; }

private:
	const long address;
};

class TestShellFullReadCmd : public TestShellCmdInterface {
public:
	TestShellFullReadCmd() {}

	void run() override {
		std::cout << "[Full Read] \n";
		for_each_addr(addr) {
			if (isError(executor->readFromSSD(addr)))
				return;
			readOutputFile(addr);
		}
	}
};
