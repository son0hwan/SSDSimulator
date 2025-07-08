#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

struct ReadRawData {
    uint32_t address;
    uint32_t data;
};

class SsdSimulator {
public:
    static SsdSimulator& getInstance() {
        static SsdSimulator instance;
        return instance;
    }

    void init() {
        // Check if nand.txt file is there
        // If not, create one? 
    }

    void write(uint32_t address, uint32_t value) {
        // Temporary code to pass UT; will be gone once parser code is in place
        if (!checkAddressRange(numOfSectors)) {
            updateOutputError();
            return;
        }
        loadDataFromNandAll();
        updateDataInInternalBuffer(address, value);
        updateNandData();
        updateOutputWriteSuccess();
    }

    void read(uint32_t address) {
        // Temporary code to pass UT; will be gone once parser code is in place
        if (!checkAddressRange(numOfSectors)) {
            updateOutputError();
            return;
        }
        loadDataFromNandAll();
        uint32_t readData = getReadData(address);
        updateOutputReadSuccess(readData);
    }

    bool checkAddressRange(uint32_t address) {
        if (address > DEFAULT_MAX_LBA_OF_DEVICE) {
            return false;
        }
        return true;
    }

    uint32_t getReadData(uint32_t address) const { 
        return readRawData[address].data; 
    }

    void updateOutputReadSuccess(uint32_t readData) {
        std::ofstream outFile(OUTPUT_FILE);
        if (!outFile) {
            throw std::exception("error opening file for writing");
        }
        outFile << "0x" << std::hex << std::uppercase << readData << std::endl;
    }

    void loadDataFromNandAll() {
        if (!readRawData.empty()) readRawData.clear();
        std::ifstream file(NAND_DATA_FILE);
        if (!file) {
            throw std::exception("error opening file for reading");
        }

        std::string line;
        FillReadRawAllDatas(file);

        file.close();
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

    void updateDataInInternalBuffer(uint32_t address, uint32_t data) {
        readRawData[address].data = data;
    }

    void updateNandData(){
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

    void updateOutputError() {
        std::ofstream outputFile(OUTPUT_FILE);
        if (!outputFile) {
            throw std::exception("error opening file for writing");
        }
        outputFile << OUTPUT_ERROR;
        outputFile.close();
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

    void FillReadRawAllDatas(std::ifstream& file)
    {
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            std::string addrStr;
            std::string dataStr;

            if (false == SplitStringToAddressAndData(line, addrStr, dataStr)) continue;
            if (false == CheckParsingLineSuccessAndPushReadRawData(line, addrStr, dataStr)) continue;
        }
    }

    bool SplitStringToAddressAndData(std::string& line, std::string& addrStr, std::string& dataStr)
    {
        size_t delimiterPos = line.find(';');
        if (delimiterPos == std::string::npos) return false;

        addrStr = line.substr(0, delimiterPos);
        dataStr = line.substr(delimiterPos + 1);
        return true;
    }

    bool CheckParsingLineSuccessAndPushReadRawData(const std::string& line, const std::string& addrStr, const std::string& dataStr)
    {
        try {
            uint32_t  addr = std::stoul(addrStr, nullptr, 16);
            uint32_t  data = std::stoul(dataStr, nullptr, 16);

            readRawData.push_back({ addr, data });
        }
        catch (const std::exception& e) {
            std::cerr << "파싱 실패: " << e.what() << ", line=" << line << std::endl;
            return false;
        }
        return true;
    }

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    uint32_t numOfSectors = DEFAULT_MAX_LBA_OF_DEVICE;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string SEPARATOR = ";";
    const std::string OUTPUT_ERROR = "ERROR";

    std::vector<ReadRawData> readRawData;
};