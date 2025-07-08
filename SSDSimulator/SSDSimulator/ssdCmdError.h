#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"

class SsdErrorCmd : public SsdCmdInterface {
public:
    SsdErrorCmd(long address) : address(address) {}

    void run() override;

private:
    const long address;
};