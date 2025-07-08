////ssdInterface.h
#pragma once
#include <cstdint>

class SsdCmdInterface {
public:
	virtual void run() = 0;
};

class SsdErrorCmd : public SsdCmdInterface {
public:
	SsdErrorCmd(long address) : address(address) {}

	void run() override {}
	long getAddress() { return address; }

private:
	const long address;
};

struct ReadRawData {
	uint32_t address;
	uint32_t data;
};