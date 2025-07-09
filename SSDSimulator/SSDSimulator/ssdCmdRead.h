#pragma once
#include "ssdInterface.h"

class SsdReadCmd : public SsdCmdInterface {
public:
    SsdReadCmd()
        : requestedAddress(0), readData(0) {}
    SsdReadCmd(uint32_t address)
        : requestedAddress(address), readData(0) {}

    void run() override;
    void setAddress(uint32_t newAddress);
    uint32_t getAddress() const;
    uint32_t getReadData() const;
    BufferedCmdInfo* getBufferedCmdInfo(void) override;
protected:
    uint32_t requestedAddress{};
    uint32_t readData{};
};


class SsdCachedReadCmd : public SsdReadCmd {
public:
    SsdCachedReadCmd(uint32_t address, uint32_t cachedData)
        : SsdReadCmd(address) {
        readData = cachedData;
    }

    void run() override;
};
