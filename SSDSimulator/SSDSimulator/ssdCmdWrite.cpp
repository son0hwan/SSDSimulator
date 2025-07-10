#include "ssdCmdWrite.h"
#include "bufferedCmdInfo.h"

void SsdWriteCmd::run() {
    SsdSimulator::getInstance().write(this->requestedAddress, this->data);
}

void SsdWriteCmd::setAddress(uint32_t newAddress) {
    requestedAddress = newAddress;
}

void SsdWriteCmd::setWriteData(uint32_t newWriteData) {
    data = newWriteData;
}

uint32_t SsdWriteCmd::getAddress() const{
    return requestedAddress; 
}
uint32_t SsdWriteCmd::getData() const {
    return data; 
}

BufferedCmdInfo* SsdWriteCmd::getBufferedCmdInfo(void) {
    return new BufferedCmdInfo(shared_from_this());
}

void SsdCachedWriteCmd::run() {
    SsdSimulator::getInstance().getIoManager().output().updateWriteSuccess();
}
