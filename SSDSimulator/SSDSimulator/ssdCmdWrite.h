#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

class SsdWriteCmd : public SsdCmdInterface {
public:
    static SsdWriteCmd& getInstance() {
        static SsdWriteCmd instance;
        return instance;
    }

    //SsdWriteCmd(uint32_t address, uint32_t data) : requestedAddress(address), data(data) {}

    void run() override;
    void setAddress(uint32_t newAddress);
    void setWriteData(uint32_t newWriteData);   
    uint32_t getAddress();
    uint32_t getData() const;

private:
    SsdWriteCmd() : requestedAddress() {}
    SsdWriteCmd(const SsdWriteCmd&) = delete;
    SsdWriteCmd& operator=(const SsdWriteCmd&) = delete;

    uint32_t requestedAddress;
    uint32_t data;
};