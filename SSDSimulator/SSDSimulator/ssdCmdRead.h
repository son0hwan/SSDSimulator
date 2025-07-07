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
        if (newAddress < 0 || newAddress > 99) {
            throw std::invalid_argument("range를 벗어났습니다.");
        }
        requestedAddress = newAddress;
    }
    
    void run() override {
        if (requestedAddress > 99 ||
            requestedAddress < 0) {
            throw std::invalid_argument("range를 벗어났습니다.");
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