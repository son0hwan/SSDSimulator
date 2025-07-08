#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdWriteCmd : public SsdCmdInterface {
public:
    static SsdWriteCmd& getInstance() {
        static SsdWriteCmd instance;
        return instance;
    }

    void run() override;
    void setAddress(uint32_t newAddress);
    void setWriteData(uint32_t newWriteData);   
    uint32_t getAddress();
    uint32_t getData() const;

private:
    SsdWriteCmd() : requestedAddress() {}
    SsdWriteCmd(const SsdWriteCmd&) = delete;
    SsdWriteCmd& operator=(const SsdWriteCmd&) = delete;

    uint32_t requestedAddress = 0;
    uint32_t data = 0;
};