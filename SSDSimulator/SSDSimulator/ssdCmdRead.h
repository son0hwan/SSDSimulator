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
        if (newAddress < 0 || newAddress > 99) {
            throw std::invalid_argument("range�� ������ϴ�.");
        }
        requestedAddress = newAddress;
    }
    
    void run() override {
        if (requestedAddress > 99 ||
            requestedAddress < 0) {
            throw std::invalid_argument("range�� ������ϴ�.");
        }
    }
    long getAddress() const { return requestedAddress; }

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    long requestedAddress;
    long readData;
};