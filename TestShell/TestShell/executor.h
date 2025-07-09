#pragma once
#include <string>
#include "randomGenerator.h"
	
#define for_each_addr(addr) for (int addr = 0; addr < NUM_OF_LBA; addr++)

class Executor {
public:
	Executor() {
		randomGenerator = new RealRandomGenerator();
	}
	virtual std::string readFromSSD(int address) = 0;
	virtual std::string writeToSSD(int address, int value) = 0;
	virtual std::string eraseToSSD(int address, int size) = 0;
	
	void setRandomGenerator(IRandomGenerator* randomGenerator) {
		this->randomGenerator = randomGenerator;
	}
	unsigned int rand(void) {
		return randomGenerator->next();
	}
private:
	IRandomGenerator* randomGenerator;
};