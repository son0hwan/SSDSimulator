#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdEraseCmd : public SsdCmdInterface, public std::enable_shared_from_this<SsdEraseCmd> {
public:
    SsdEraseCmd()
        : startAddress(0), eraseSize(0) {}

    SsdEraseCmd(uint32_t address, uint32_t size)
        : startAddress(address), eraseSize(size) {}

    void run() override;
    void setStartAddress(uint32_t newAddress);
    void setEraseSize(uint32_t eSize);

    BufferedCmdInfo* getBufferedCmdInfo(void);

    uint32_t getStartAddress() const;
    uint32_t getSize() const;

private:
    uint32_t startAddress;
    uint32_t eraseSize;
};