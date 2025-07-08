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
    void setAddress(uint32_t newAddress) {
        CheckAddressRange(newAddress);
        requestedAddress = newAddress;
    }

    void run() override {
        CheckAddressRange(requestedAddress);
    }
    uint32_t getAddress() const { return requestedAddress; }

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    void CheckAddressRange(uint32_t newAddress);

    uint32_t requestedAddress;
    uint32_t readData;
};