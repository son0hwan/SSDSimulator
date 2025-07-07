#include "executor.h"
class TestShell {
public:
	TestShell(Executor* executor) : executor(executor) {}
	int write(int address, int value) {
		executor->writeToSSD(address, value);
		return 0;
	}
	
private:
	Executor* executor;
};