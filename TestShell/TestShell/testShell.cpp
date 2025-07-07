#include "executor.h"
class TestShell {
public:
	TestShell(Executor* executor) : executor(executor) {}
	int write(int address, int value) {
		std::string result;
		result = executor->writeToSSD(address, value);
		if (result == "ERROR")
			return 1;

		return 0;
	}
	
private:
	Executor* executor;
};