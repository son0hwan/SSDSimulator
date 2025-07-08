#pragma once
#include "ssdInterface.h"
#include <string>
#include <vector>
#include <stdexcept>

class SsdReadCmd : public SsdCmdInterface {
public:
    // Singleton �ν��Ͻ��� ��ȯ
    static SsdReadCmd& getInstance() {
        static SsdReadCmd instance;
        return instance;
    }

    // �ּ� �缳�� �Լ�
    void setAddress(long newAddress) {
        CheckAddressRange(newAddress);
        requestedAddress = newAddress;
    }

    void run() override {
        CheckAddressRange(requestedAddress);
    }
    long getAddress() const { return requestedAddress; }

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    void CheckAddressRange(long newAddress);

    long requestedAddress;
    long readData;
};