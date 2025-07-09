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

private:
    uint32_t requestedAddress{};
    uint32_t readData{};
};
