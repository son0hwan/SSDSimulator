////ssdInterface.h
#pragma once
#include <cstdint>

class SsdCmdInterface {
public:
    virtual void run() = 0;
};

struct ReadRawData {
    uint32_t address;
    uint32_t data;
};