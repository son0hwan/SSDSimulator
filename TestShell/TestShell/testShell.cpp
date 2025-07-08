#include "executor.h"

#define MAX_LBA 100

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
		for (int sector = 0; sector < MAX_LBA; sector++) {
			if (executor->readFromSSD(sector) == "ERROR")
				return false;
		}
		return true;
	}
	
private:
	Executor* executor;
};