#pragma once
#include "ssdInterface.h"
#include "ssdSimulator.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

class SsdWriteCmd : public SsdCmdInterface {
public:
    static SsdWriteCmd& getInstance() {
        static SsdWriteCmd instance;
        return instance;
    }

    //SsdWriteCmd(uint32_t address, uint32_t data) : requestedAddress(address), data(data) {}

    void run() override;
    void setAddress(uint32_t newAddress);
    void setWriteData(uint32_t newWriteData);
    void readNandData(const std::string& filename);
    void updateOutputWriteSuccess();
    void updateOutputError();
    void updateNandData();
    void WriteSectorAddressAndDataToNAND(std::ofstream& nandDataFile, uint32_t addr);
    uint32_t getAddress() { return requestedAddress; }
    uint32_t getValue() { return data;  }
    uint32_t getData() const { return data; }

private:
    SsdWriteCmd() : requestedAddress() {}
    SsdWriteCmd(const SsdWriteCmd&) = delete;
    SsdWriteCmd& operator=(const SsdWriteCmd&) = delete;

    uint32_t requestedAddress;
    uint32_t data;
    static const int DEVICE_MAX_ADDRESS = 99;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string SEPARATOR = ";";
    const std::string OUTPUT_ERROR = "ERROR";

    bool CheckAddressRange(uint32_t address);
    void loadDataFromNand(const std::string& filename);
    void updateDataInInternalBuffer(uint32_t address, uint32_t data);

    std::vector<ReadRawData> readRawData;
};