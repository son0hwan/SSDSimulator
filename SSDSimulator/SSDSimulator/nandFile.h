#pragma once
#include "fileIo.h"

struct LbaEntry {
    uint32_t address;
    uint32_t data;
};

class NandFile {
public:
    NandFile() : maxLbaOfDevice(0) {};
    NandFile(uint32_t maxLbaOfDevice) : maxLbaOfDevice(maxLbaOfDevice) {}

    void CheckAndCreateNandDataFile() {
        if (nandDataFileExist()) return;
        auto nandDataFile = openFile(NAND_DATA_FILE);
        FillZeroDataToAllAddresses(nandDataFile);
    }

    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable) {
        auto nandDataFile = openFile(NAND_DATA_FILE);
        FillChangeDatasToAllAddresses(nandDataFile, lbaTable);
    }

    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable) {
        std::ifstream file(NAND_DATA_FILE);
        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;

            LbaEntry splitDatas;
            if (false == SplitStringToAddressAndData(line, &splitDatas)) continue;
            lbaTable.push_back({ splitDatas.address, splitDatas.data });
        }
    }

    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas) {
        std::string addrStr, dataStr;
        size_t delimiterPos = line.find(SEPARATOR.c_str());
        if (delimiterPos == std::string::npos) return false;

        addrStr = line.substr(0, delimiterPos);
        dataStr = line.substr(delimiterPos + 1);

        return SuccessConvertToUINT(splitDatas, addrStr, dataStr);
    }

    void CreateNewTempNandFileAndInitForTest() {
        std::ifstream file(NAND_DATA_FILE);
        if (!file) {
            std::vector<LbaEntry> readDatas{};
            uint32_t initAddress = 0x705FF427;
            for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
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

private:
    uint32_t maxLbaOfDevice;
    const static uint32_t INIT_NAND_DATA = 0;
    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string SEPARATOR = ";";

    std::ofstream openFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("error opening file");
        }
        return file;
    }

    bool nandDataFileExist() {
        std::ifstream file(NAND_DATA_FILE);
        if (!file) return false;
        return true;
    }

    void FillZeroDataToAllAddresses(std::ofstream& nandDataFile) {
        for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
            nandDataFile << std::hex << lba;
            nandDataFile << SEPARATOR;
            nandDataFile << std::setw(8) << std::setfill('0') << INIT_NAND_DATA;
            nandDataFile << std::endl;
        }
    }

    void FillChangeDatasToAllAddresses(std::ofstream& nandDataFile,
        const std::vector<LbaEntry>& lbaTable) {
        for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
            nandDataFile << std::hex << lba;
            nandDataFile << SEPARATOR;
            nandDataFile << std::hex << std::setw(8) << std::setfill('0')
                << lbaTable[lba].data;
            nandDataFile << std::endl;
        }
    }

    bool SuccessConvertToUINT(LbaEntry* splitDatas, std::string& addrStr,
        std::string& dataStr) {
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
};