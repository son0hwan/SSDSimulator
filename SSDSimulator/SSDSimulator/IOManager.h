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

    void CheckAndCreateNandDataFile();
    void updateOutputError();
    void updateOutputWriteSuccess();
    void updateOutputReadSuccess(uint32_t readData);
    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);
    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable);
    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
    void CreateNewTempNandFileAndInitForTest();
    void deleteFileIfExists();

    bool forceCreateFiveFreshBufferFiles();
    bool updateBufferFiles(const std::vector<std::string> buffers);
    std::vector<std::string> getBufferFileList();

private:
    bool nandDataFileExist();
    std::ofstream openFile(const std::string& filename);
    void FillZeroDataToAllAddresses(std::ofstream& nandDataFile);
    void FillChangeDatasToAllAddresses(std::ofstream& nandDataFile,
        const std::vector<LbaEntry>& lbaTable);
    bool SuccessConvertToUINT(LbaEntry* splitDatas, std::string& addrStr,
        std::string& dataStr);
    
    bool createFiveBufferFiles(const std::vector<std::string> buffers);
    bool createFiveFreshBufferFiles();
    void createEmptyBufferFolder();
    void createBufferFolder();
    bool removeAllFilesInFolder();
    bool createBufferFile(const std::string& fileName);
    std::string removeFolderPathFrom(const std::filesystem::directory_entry& file);
    bool fileExists(const std::string fileName);

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