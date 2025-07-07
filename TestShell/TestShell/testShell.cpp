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
	bool fullRead() {
		for (int i = 0; i < 100; i++) {
			if (executor->readFromSSD(i) == "ERROR")
				return false;
		}
		return true;
	}
	
private:
	Executor* executor;
};