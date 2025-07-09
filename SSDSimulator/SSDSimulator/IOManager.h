#pragma once
#include "outputFile.h"

namespace fs = std::filesystem;

struct LbaEntry {
    uint32_t address;
    uint32_t data;
};

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : maxLbaOfDevice(maxLbaOfDevice) {}

    OutputFile& output() { return outputFile; }
#if 1 /*Output-related functions <-- Need to be gone later*/
    void updateOutputError();
    void updateOutputWriteSuccess();
    void updateOutputReadSuccess(uint32_t readData);
    // OpenFile 
#endif

    void CheckAndCreateNandDataFile();
    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);
    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable);
    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
    void CreateNewTempNandFileAndInitForTest();
    void deleteFileIfExists();

    bool forceCreateFiveFreshBufferFiles();
    bool updateBufferFiles(const std::vector<std::string> buffers);
    std::vector<std::string> getBufferFileList();

private:
    OutputFile outputFile;

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
    const std::string SEPARATOR = ";";

    static const uint32_t NUM_OF_BUFFERS = 5;
    const std::string BUFFER_FOLDER = "./buffer/";
    const std::string EMPTY_BUFFER_FILE_SUFFIX = "_empty";
};