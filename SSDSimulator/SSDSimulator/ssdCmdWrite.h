#pragma once
#include "ssdInterface.h"

class SsdWriteCmd : public SsdCmdInterface {
public:
    // 생성자는 parser에서 불러줄 것
    SsdWriteCmd(long address, long data) : address(address), data(data) {}

    void run() override;

private:
    const long address;
    const long data;
};