#pragma once
#include "executor.h"

class TestShell {
public:
	TestShell(Executor* executor) : executor(executor) {}
	int write(int address, int value) {
		executor->writeToSSD(address, value);
		return 0;
	}
	std::string read(int address) {
		return executor->readFromSSD(address);
	}
	std::string fullRead() {
		return "";
	}
	
private:
	Executor* executor;
};