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
        readNandData("ssd_nand.txt");
    }
    long getAddress() const { return requestedAddress; }
    long getReadData() const { return readData; }
    void readNandData(const std::string& filename);

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    void CheckAddressRange(long newAddress);
    void ParseFile(const std::string& filename);

    long requestedAddress;
    long readData;

    std::vector<ReadRawData> readRawData;
};