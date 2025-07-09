#pragma once
#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdEraseCmd : public SsdCmdInterface {
public:
    static SsdEraseCmd& getInstance() {
        static SsdEraseCmd instance;
        return instance;
    }

    void run() override;
    void setStartAddress(uint32_t newAddress);
    void setEraseSize(uint32_t eSize);
    bool isAllowBuffering() override { return true; }

   private:
    SsdEraseCmd() : startAddress() {}
    SsdEraseCmd(const SsdEraseCmd&) = delete;
    SsdEraseCmd& operator=(const SsdEraseCmd&) = delete;

    uint32_t startAddress = 0;
    uint32_t eraseSize = 0;
};