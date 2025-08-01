#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdWriteCmd : public SsdCmdInterface, public std::enable_shared_from_this <SsdWriteCmd>{
public:
	SsdWriteCmd()
		: requestedAddress(0), data(0) {}

	SsdWriteCmd(uint32_t address, uint32_t writeData)
		: requestedAddress(address), data(writeData) {}

	void run() override;
	void setAddress(uint32_t newAddress);
	void setWriteData(uint32_t newWriteData);
	uint32_t getAddress() const;
	uint32_t getData() const;
	BufferedCmdInfo* getBufferedCmdInfo(void);

private:
	uint32_t requestedAddress;
	uint32_t data;
};

class SsdCachedWriteCmd : public SsdCmdInterface {
public:
	SsdCachedWriteCmd() {}

	void run() override;
	BufferedCmdInfo* getBufferedCmdInfo(void) { return nullptr; }
};
