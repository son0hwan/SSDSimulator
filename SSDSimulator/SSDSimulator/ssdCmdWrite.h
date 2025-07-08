#pragma once
#include "ssdInterface.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

class SsdWriteCmd : public SsdCmdInterface {
public:
    // 생성자는 parser에서 불러줄 것
    SsdWriteCmd(uint32_t address, uint32_t data) : address(address), data(data) {}

    void run() override;
    void setAddress(uint32_t newAddress);
    void setWriteData(uint32_t newWriteData);
    void readNandData(const std::string& filename);
    void updateOutput();
    void updateOutputError();
    void updateNandData();
    void WriteSectorAddressAndDataToNAND(std::ofstream& nandDataFile, uint32_t addr);


private:
    uint32_t address;
    uint32_t data;
    static const int DEVICE_MAX_ADDRESS = 99;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string SEPARATOR = ";";
    const std::string OUTPUT_ERROR = "ERROR";

    bool CheckAddressRange(uint32_t address);
    void ParseFile(const std::string& filename);
    void updateDataInInternalBuffer(uint32_t address, uint32_t data);

    std::vector<ReadRawData> readRawData;
};