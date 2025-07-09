#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

struct LbaEntry {
    uint32_t address;
    uint32_t data;
};

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : maxLbaOfDevice( maxLbaOfDevice ) {}

    void CheckAndCreateNandDataFile() {
        if (nandDataFileExist()) return;

        auto nandDataFile = openFile(NAND_DATA_FILE);
        FillZeroDataToAllAddresses(nandDataFile);
    }

    void updateOutputError() {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << OUTPUT_ERROR;
    }
    
    void updateOutputWriteSuccess() {
        auto outputFile = openFile(OUTPUT_FILE);
    }

    void updateOutputReadSuccess(uint32_t readData) {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << "0x" << std::hex << std::uppercase << readData << std::endl;
    }

    void ProgramAllDatasToNand(const std::vector<LbaEntry> & lbaTable) {
        auto nandDataFile = openFile(NAND_DATA_FILE);
        FillChangeDatasToAllAddresses(nandDataFile, lbaTable);
    }

    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable)
    {
        std::ifstream file(NAND_DATA_FILE);
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            LbaEntry splitDatas;
            if (false == SplitStringToAddressAndData(line, &splitDatas)) continue;
            lbaTable.push_back({ splitDatas.address, splitDatas.data });
        }
    }

    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas)
    {
        std::string addrStr, dataStr;
        size_t delimiterPos = line.find(SEPARATOR.c_str());
        if (delimiterPos == std::string::npos) return false;

        addrStr = line.substr(0, delimiterPos);
        dataStr = line.substr(delimiterPos + 1);

        return SuccessConvertToUINT(splitDatas, addrStr, dataStr);
    }

#ifdef _DEBUG
    void CreateNewTempNandFileAndInitForTest()
    {
        std::ifstream file(NAND_DATA_FILE);
        if (!file) {
            std::vector<LbaEntry> readDatas{};
            uint32_t initAddress = 0x705FF427;
            for (uint32_t lba = 0; lba <= DEFAULT_MAX_LBA_OF_DEVICE; ++lba) {
                readDatas.push_back({ lba, initAddress++ });
            }
            ProgramAllDatasToNand(readDatas);
        }
    }

    void deleteFileIfExists() {
        if (std::ifstream(NAND_DATA_FILE)) {
            std::remove(NAND_DATA_FILE.c_str()); 
        }
    }
#endif

private:
    bool nandDataFileExist() {
        std::ifstream file(NAND_DATA_FILE);
        if (!file) return false;
        return true;
    }

    std::ofstream openFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("error opening file");
        }
        return file;
    }

    void FillZeroDataToAllAddresses(std::ofstream& nandDataFile)
    {
        for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
            nandDataFile << std::hex << lba;
            nandDataFile << SEPARATOR;
            nandDataFile << std::setw(8) << std::setfill('0') << INIT_NAND_DATA;
            nandDataFile << std::endl;
        }
    }

    void FillChangeDatasToAllAddresses(std::ofstream& nandDataFile, const std::vector<LbaEntry>& lbaTable)
    {
        for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
            nandDataFile << std::hex << lba;
            nandDataFile << SEPARATOR;
            nandDataFile << std::hex << std::setw(8) << std::setfill('0') << lbaTable[lba].data;
            nandDataFile << std::endl;
        }
    }

    bool SuccessConvertToUINT(LbaEntry* splitDatas, std::string& addrStr, std::string& dataStr)
    {
        try {
            (*splitDatas).address = std::stoul(addrStr, nullptr, 16);
            (*splitDatas).data = std::stoul(dataStr, nullptr, 16);
        }
        catch (const std::exception& e) {
            std::cerr << "Fail convert string to unsigned long :";
            std::cerr << e.what() << std::endl;
            return false;
        }
        return true;
    }

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    const uint32_t maxLbaOfDevice;
    const static uint32_t INIT_NAND_DATA = 0;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string OUTPUT_ERROR = "ERROR";
    const std::string SEPARATOR = ";";
};