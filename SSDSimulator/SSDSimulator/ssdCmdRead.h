#pragma once
#include "ssdInterface.h"

class SsdReadCmd : public SsdCmdInterface {
public:
    // �����ڴ� parser���� �ҷ��� ��
    SsdReadCmd(long address) : address(address) {}

    void run() override {}
    long getAddress() { return address; }

private:
    const long address;
};