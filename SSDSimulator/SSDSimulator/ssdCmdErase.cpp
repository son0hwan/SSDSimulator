#include "ssdCmdErase.h"
#include "bufferedCmdInfo.h"

void SsdEraseCmd::run() {
    SsdSimulator::getInstance().erase(this->startAddress, this->eraseSize);
}

void SsdEraseCmd::setStartAddress(uint32_t newAddress) {
    startAddress = newAddress;
}

void SsdEraseCmd::setEraseSize(uint32_t eSize) {
    eraseSize = eSize;
}

uint32_t SsdEraseCmd::getStartAddress() const{
    return startAddress;
}
uint32_t SsdEraseCmd::getSize() const {
    return eraseSize;
}

BufferedCmdInfo* SsdEraseCmd::getBufferedCmdInfo(void) {
    return new BufferedCmdInfo(shared_from_this());
}