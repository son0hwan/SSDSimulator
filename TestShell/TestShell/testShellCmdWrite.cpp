#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
public:
	TestShellWriteCmd(long address, unsigned data)
		: address(address), data(data) {
	}

	void run() override {
		std::string result;

		result = executor->writeToSSD(address, data);
		if (result == ERROR_STRING)
			return;
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
		std::string result;

		for (int addr = 0; addr < NUM_OF_LBA; addr++) {
			result = executor->writeToSSD(addr, data);
			if (result == ERROR_STRING)
				break;
		}
	}
private:
	const unsigned data;
};