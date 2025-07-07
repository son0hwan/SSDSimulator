#include <string>

class Executor {
	virtual std::string readFromSSD(int address) = 0;
	virtual std::string writeToSSD(int address, int value) = 0;
};