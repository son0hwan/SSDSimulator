#pragma once
#include <string>

class Executor {
public:
	virtual std::string readFromSSD(int address) = 0;
	virtual std::string writeToSSD(int address, int value) = 0;
};