#include "ssdCmdWrite.h"

void SsdWriteCmd::run() {
    SsdSimulator::getInstance().write(this->requestedAddress, this->data);
}

void SsdWriteCmd::setAddress(uint32_t newAddress) {
    requestedAddress = newAddress;
}

void SsdWriteCmd::setWriteData(uint32_t newWriteData) {
    data = newWriteData;
}

uint32_t SsdWriteCmd::getAddress() {
    return requestedAddress; 
}
uint32_t SsdWriteCmd::getData() const {
    return data; 
}