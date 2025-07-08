#pragma once
#include "ssdInterface.h"
#include <string>
#include <vector>
#include <stdexcept>
#include "ssdSimulator.h"

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
        readNandData("ssd_nand.txt");

        //SsdSimulator::getInstance().read(requestedAddress);
    }
    uint32_t getAddress() const { return requestedAddress; }
    uint32_t getReadData() const { return readData; }
    void readNandData(const std::string& filename);

private:
    SsdReadCmd() : requestedAddress() {}
    SsdReadCmd(const SsdReadCmd&) = delete;
    SsdReadCmd& operator=(const SsdReadCmd&) = delete;

    void CheckAddressRange(uint32_t newAddress);
    void ParseFile(const std::string& filename);

    void FillReadRawAllDatas(std::ifstream& file);

    void CheckInputAndCreateInputFile(std::ifstream& file, const std::string& filename);
    void ReOpenNandFile(std::ifstream& file, const std::string& filename);
    void CreateNewNandFile(const std::string& filename);
    void FillZeroDataToAllAddresses(std::ofstream& newFile);
    bool CheckParsingLineSuccessAndPushReadRawData(const std::string& line, const std::string& addrStr, const std::string& dataStr);
    bool SplitStringToAddressAndData(std::string& line, std::string& addrStr, std::string& dataStr);

    uint32_t requestedAddress;
    uint32_t readData;

    std::vector<ReadRawData> readRawData;

    static const std::string OUTPUT_FILENAME;
    static const std::string OUTPUT_ERROR;
};
