#pragma once
#include <direct.h>
#include <errno.h>
#include <sys/types.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

struct LbaEntry {
    uint32_t address;
    uint32_t data;
};

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : maxLbaOfDevice(maxLbaOfDevice) {}

    void CheckAndCreateNandDataFile() {
        if (nandDataFileExist()) return;

        auto nandDataFile = openFile(NAND_DATA_FILE);
        FillZeroDataToAllAddresses(nandDataFile);
    }

    void updateOutputError() {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << OUTPUT_ERROR;
    }

    void updateOutputWriteSuccess() { auto outputFile = openFile(OUTPUT_FILE); }

    void updateOutputReadSuccess(uint32_t readData) {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << "0x" << std::uppercase << std::hex << std::setw(8)
            << std::setfill('0') << readData << std::endl;
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

    bool forceCreateFiveFreshBufferFiles() {
        createEmptyBufferFolder();
        return createFiveFreshBufferFiles();
    }

    bool updateBufferFiles(const std::vector<std::string> buffers) {
        createEmptyBufferFolder();
        return createFiveBufferFiles(buffers);
    }

    std::vector<std::string> getBufferFileList() {
        std::vector<std::string> buffers;
        for (const auto& fileName : std::filesystem::directory_iterator(BUFFER_FOLDER)) {
            if (fileName.is_regular_file()) {
                buffers.push_back(removeFolderPathFrom(fileName));
            }
        }
        return buffers;
    }

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

    bool createFiveBufferFiles(const std::vector<std::string> buffers) {
        for (auto bufferFileName : buffers) {
            if (!createBufferFile(BUFFER_FOLDER + bufferFileName)) return false;
        }
        return true;
    }

    bool createFiveFreshBufferFiles() {
        for (unsigned int buffer = 1; buffer <= NUM_OF_BUFFERS; buffer++) {
            std::string fileName = std::to_string(buffer) + EMPTY_BUFFER_FILE_SUFFIX;
            if (!createBufferFile(BUFFER_FOLDER + fileName)) return false;
        }
        return true;
    }

    void createEmptyBufferFolder()
    {
        createBufferFolder();
        removeAllFilesInFolder();
    }

    void createBufferFolder() {
        std::filesystem::create_directory(BUFFER_FOLDER);
    }

    bool removeAllFilesInFolder() {
        try {
            for (const auto& entry : fs::directory_iterator(BUFFER_FOLDER)) {
                if (entry.is_regular_file()) {
                    fs::remove(entry.path());
                }
            }
            return true;
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error removing files: " << e.what() << std::endl;
            return false;
        }
    }

    bool createBufferFile(const std::string& fileName) {
        std::ofstream file(fileName, std::ios::trunc);  // trunc: overwrite if exists
        return file.is_open();
    }

    std::string removeFolderPathFrom(const std::filesystem::directory_entry& file) {
        return file.path().filename().string();
    }

    bool fileExists(const std::string fileName) {
        fs::path filePath = fs::path(BUFFER_FOLDER) / fileName;
        if (!fs::exists(filePath)) {
            std::cerr << "File not found: " << filePath << std::endl;
            return false;
        }
        return true;
    }

    const uint32_t maxLbaOfDevice;
    const static uint32_t INIT_NAND_DATA = 0;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string OUTPUT_ERROR = "ERROR";
    const std::string SEPARATOR = ";";

    static const uint32_t NUM_OF_BUFFERS = 5;
    const std::string BUFFER_FOLDER = "./buffer/";
    const std::string EMPTY_BUFFER_FILE_SUFFIX = "_empty";
};