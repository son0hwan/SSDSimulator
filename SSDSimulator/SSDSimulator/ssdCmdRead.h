#pragma once
#include "ssdInterface.h"
#include <string>
#include <vector>
#include <stdexcept>

class SsdReadCmd : public SsdCmdInterface {
public:
    static SsdReadCmd& getInstance() {
        static SsdReadCmd instance;
        return instance;
    }

    void run() override;
    void setAddress(uint32_t newAddress);
    uint32_t getAddress() const;
    uint32_t getReadData() const;

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    uint32_t requestedAddress;
    uint32_t readData;
};
