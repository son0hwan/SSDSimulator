#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
public:
	TestShellWriteCmd(long address, unsigned data)
		: address(address), data(data) {
	}

	void run() override {
		if (isError(executor->writeToSSD(address, data))) {
			std::cout << "[Write] Error" << std::endl;
			return;
		}
		std::cout << "[Write] Done" << std::endl;
	}

	long getAddress() { return address; }
	long getData() { return data; }

private:
	const long address;
	const unsigned data;
};

class TestShellFullWriteCmd : public TestShellCmdInterface {
public:
	TestShellFullWriteCmd(unsigned data)
		: data (data) {
	}

	void run() override {
		for_each_addr(addr) {
			if (isError(executor->writeToSSD(addr, data))) {
				std::cout << "[FullWrite] Error" << std::endl;
				return;
			}
		}

		std::cout << "[FullWrite] Done" << std::endl;
	}
private:
	const unsigned data;
};