#pragma once
#include "executor.h"

namespace {
	const int WRITE_SUCCESS = 0;
	const int WRITE_ERROR = 1;
	const char* WRITE_SUCCESS_STRING = "";
	const char* WRITE_ERROR_STRING = "ERROR";
	const int NUM_OF_LBA = 100;
	const int MAX_VAL_LEN = 8;
	const int MAX_ALPHABET_SIZE = 26;
}

class TestShell {
public:
	TestShell(Executor* executor) : executor(executor) {}
	int write(int address, int value) {
		std::string result;
		result = executor->writeToSSD(address, value);
		if (result == WRITE_ERROR_STRING)
			return WRITE_ERROR;

		return WRITE_SUCCESS;
	}

	int fullwrite(int value) {
		std::string result;

		for (int addr = 0; addr < NUM_OF_LBA; addr++) {
			result = executor->writeToSSD(addr, value);
			if (result == WRITE_ERROR_STRING)
				return WRITE_ERROR;
		}

		return WRITE_SUCCESS;
	}
	std::string read(int address) {
		return executor->readFromSSD(address);
	}
	bool fullRead() {
		for (int addr = 0; addr < NUM_OF_LBA; addr++) {
			if (executor->readFromSSD(addr) == WRITE_ERROR_STRING)
				return false;
		}
		return true;
	}
	
private:
	Executor* executor;
};