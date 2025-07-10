#include "ssdCmdRead.h"
#include "bufferedCmdInfo.h"

void SsdReadCmd::setAddress(uint32_t newAddress) {
	requestedAddress = newAddress;
}

void SsdReadCmd::run() {
	readData = SsdSimulator::getInstance().read(requestedAddress);
}

uint32_t SsdReadCmd::getAddress() const {
	return requestedAddress;
}

uint32_t SsdReadCmd::getReadData() const {
	return readData;
}
BufferedCmdInfo* SsdReadCmd::getBufferedCmdInfo(void) {
	return new BufferedCmdInfo(this);
}

void SsdCachedReadCmd::run(void) {
	SsdSimulator::getInstance().getIoManager().output().updateReadSuccess(readData);
}