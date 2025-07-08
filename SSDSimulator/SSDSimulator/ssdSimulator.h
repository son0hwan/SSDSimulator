#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
#include "ssdInterface.h"

//struct ReadRawData {
//    uint32_t address;
//    uint32_t data;
//};

class SsdSimulator {
public:
    static SsdSimulator& getInstance() {
        static SsdSimulator instance;
        return instance;
    }

    void write(uint32_t address, uint32_t value) {
        if (!checkAddressRange(numOfSectors))return;
        readNandData();
        updateDataInInternalBuffer(address, value);
        updateNandData();
        updateOutputWriteSuccess();
    }

    void read(uint32_t address) {

    }

    bool checkAddressRange(uint32_t address) {
        if (address < 0 || address > DEFAULT_MAX_LBA_OF_DEVICE) {
            return false;
        }
        return true;
    }

    void readNandData() {
        if (!readRawData.empty()) readRawData.clear();
        loadDataFromNand(NAND_DATA_FILE);
#if 0
        // address와 일치하는 데이터를 찾아서 readData에 저장
        auto foundReadData = std::find_if(
            readRawData.begin(),
            readRawData.end(),
            [this](const ReadRawData& readEntry) {
                return readEntry.address == requestedAddress;
            }
        );

        if (foundReadData != readRawData.end()) {
            data = foundReadData->data;
            std::ofstream outFile("ssd_output.txt");
            outFile << "0x" << std::hex << std::uppercase << data << std::endl;
            return;
        }
        else {
            std::ofstream outFile("ssd_output.txt");
            outFile << "ERROR" << std::endl;
            return;
        }
#endif
    }

    void loadDataFromNand(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::exception("error opening file for reading");
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue; // 공백 줄 스킵

            size_t delimiterPos = line.find(';');
            if (delimiterPos == std::string::npos) continue; // 세미콜론 없으면 스킵

            std::string addrStr = line.substr(0, delimiterPos);
            std::string dataStr = line.substr(delimiterPos + 1);

            try {
                uint32_t  addr = std::stoul(addrStr, nullptr, 16);  // 16진수 주소 처리
                uint32_t  data = std::stoul(dataStr, nullptr, 16);  // 16진수 데이터 처리

                readRawData.push_back({ addr, data });
            }
            catch (const std::exception& e) {
                std::cerr << "파싱 실패: " << e.what() << ", line=" << line << std::endl;
                continue; // 실패한 라인은 스킵
            }
        }

        file.close();
    }

    void updateDataInInternalBuffer(uint32_t address, uint32_t data) {
        readRawData[address].data = data;
    }

    void updateNandData()
    {
        std::ofstream nandDataFile(NAND_DATA_FILE); // Open file for writing
        if (!nandDataFile) {
            throw std::exception("error opening file for writing");
        }

        for (int addr = 0; addr < readRawData.size(); addr++) {
            nandDataFile << std::hex << addr;
            nandDataFile << SEPARATOR;
            nandDataFile << std::hex << std::setw(8) << std::setfill('0') << readRawData[addr].data << std::endl;
        }

        nandDataFile.close();
    }

    void updateOutputWriteSuccess()
    {
        std::ofstream outputFile(OUTPUT_FILE);
        if (!outputFile) {
            throw std::exception("error opening file for writing");
        }
        outputFile.close();
    }

private: 
    SsdSimulator() {};
    SsdSimulator(const SsdSimulator&) = delete;
    SsdSimulator& operator=(const SsdSimulator&) = delete;

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    uint32_t numOfSectors = DEFAULT_MAX_LBA_OF_DEVICE;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string SEPARATOR = ";";
    const std::string OUTPUT_ERROR = "ERROR";

    std::vector<ReadRawData> readRawData;
};