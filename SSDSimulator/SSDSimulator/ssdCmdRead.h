#pragma once
#include "ssdInterface.h"

class SsdReadCmd : public SsdCmdInterface {
public:
    // 생성자는 parser에서 불러줄 것
    SsdReadCmd(long address) : address(address) {}

    void run() override {}
    long getAddress() { return address; }

private:
    const long address;
};