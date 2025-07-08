#include "testShellCmdInterface.h"
#include <iostream>
#include <fstream>

class TestShellReadCmd : public TestShellCmdInterface {
public:
	TestShellReadCmd(long address) : address(address) {}

	void readOutputFile() {
		std::ifstream file("ssd_output.txt");
		
		if (!file.is_open()) {
			std::cerr << "fail to open file" << std::endl;
			return;
		}

		std::string line;
		if (std::getline(file, line)) {   
			std::cout << line << std::endl;
		}
		else {
			std::cout << "fail to read file" << std::endl;
		}

		file.close();  
	}

	void run() override {
		executor->readFromSSD(address);
		readOutputFile();
	}

	long getAddress() { return address; }

private:
	const long address;
};

class TestShellFullReadCmd : public TestShellCmdInterface {
public:
	TestShellFullReadCmd() {}

	void run() override {
		for (int addr = 0; addr < NUM_OF_LBA; addr++) {
			if (executor->readFromSSD(addr) == ERROR_STRING)
				break;
		}
	}
};