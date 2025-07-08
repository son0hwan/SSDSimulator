#pragma once
#include "executor.h"
#include <vector>

using std::vector;

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
	int write(int address, unsigned int value) {
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

	bool fullWriteAndReadCompare(const vector<unsigned int>& values) {
		for (int idx = 0; idx < values.size(); idx++) {
			int startIdx = idx * 5;
			for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
				int addr = startIdx + unitIdx;
				unsigned int value = values.at(idx);

				if (write(addr, value) == WRITE_ERROR)
					return false;
			}

			for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
				int addr = startIdx + unitIdx;
				unsigned int value = values.at(idx);
				std::string expectedValString = "0x" + std::to_string(value);
				std::string actualValString = read(addr);

				if (expectedValString != actualValString)
					return false;
			}
		}

		return true;
	}

private:
	Executor* executor;
};