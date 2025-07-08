#pragma once
#include "ssdInterface.h"
#include <string>
#include <vector>
#include <stdexcept>

class SsdReadCmd : public SsdCmdInterface {
public:
    // Singleton 인스턴스를 반환
    static SsdReadCmd& getInstance() {
        static SsdReadCmd instance;
        return instance;
    }

    // 주소 재설정 함수
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