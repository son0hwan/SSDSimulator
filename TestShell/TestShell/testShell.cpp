#include "executor.h"

namespace {
	const int WRITE_SUCCESS = 0;
	const int WRITE_ERROR = 1;
	const char* WRITE_SUCCESS_STRING = "";
	const char* WRITE_ERROR_STRING = "ERROR";
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
	
private:
	Executor* executor;
};