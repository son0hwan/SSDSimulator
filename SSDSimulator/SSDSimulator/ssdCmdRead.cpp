#include "ssdSimulator.h"
#include "ssdCmdRead.h"

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