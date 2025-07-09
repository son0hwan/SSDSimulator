#pragma once
#include <string>
#include "randomGenerator.h"
#include "common.h"
	
#define for_each_addr(addr) for (int addr = 0; addr < NUM_OF_LBA; addr++)

class Executor {
public:
	Executor() {
		randomGenerator = new RealRandomGenerator();
	}
	virtual std::string readFromSSD(int address) = 0;
	virtual std::string writeToSSD(int address, unsigned int value) = 0;
	virtual std::string eraseToSSD(int address, int size) = 0;

	int readFromSSDWithResult(int address, unsigned int* value) {
		std::string valStr = readFromSSD(address);
		if (valStr == FAIL || valStr == ERROR_STRING)
			return ERROR;

		*value = std::stoul(valStr, nullptr, 16);
		return SUCCESS;
	}
	int writeToSSDWithResult(int address, unsigned int value) {
		std::string val = writeToSSD(address, value);
		if (val == FAIL || val != SUCCESS_STRING)
			return ERROR;

		return SUCCESS;
	}
	
	void setRandomGenerator(IRandomGenerator* randomGenerator) {
		this->randomGenerator = randomGenerator;
	}
	unsigned int rand(void) {
		return randomGenerator->next();
	}
private:
	IRandomGenerator* randomGenerator;
};