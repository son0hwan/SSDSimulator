#pragma once
#include "ssdInterface.h"

class SsdWriteCmd : public SsdCmdInterface {
public:
    // �����ڴ� parser���� �ҷ��� ��
    SsdWriteCmd(long address, long data) : address(address), data(data) {}
    void run() override {}

    long getAddress() { return address; }
    long getValue() { return data;  }
    long getData() const { return data; }

private:
    const long address;
    const long data;
};