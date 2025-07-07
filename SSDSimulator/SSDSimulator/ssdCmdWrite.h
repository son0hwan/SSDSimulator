#pragma once
#include "ssdInterface.h"

class SsdWriteCmd : public SsdCmdInterface {
public:
    // �����ڴ� parser���� �ҷ��� ��
    SsdWriteCmd(long address, long data) : address(address), data(data) {}

    void run() override;

private:
    const long address;
    const long data;
};