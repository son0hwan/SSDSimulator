#pragma once
#include "outputFile.h"
#include "bufferFile.h"

struct LbaEntry {
    uint32_t address;
    uint32_t data;
};

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : maxLbaOfDevice(maxLbaOfDevice) {}

    OutputFile& output() { return outputFile; }
#if OUTPUT_REFACTORING
    void updateOutputError();
    void updateOutputWriteSuccess();
    void updateOutputReadSuccess(uint32_t readData);
    std::ofstream openFile(const std::string& filename);
#endif

    BufferFile& buffer() { return bufferFile; }
#if BUFFER_REFACTORING 
    bool forceCreateFiveFreshBufferFiles();
    bool updateBufferFiles(const std::vector<std::string> buffers);
    std::vector<std::string> getBufferFileList();
#endif

    void CheckAndCreateNandDataFile();
    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);
    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable);
    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
    void CreateNewTempNandFileAndInitForTest();
    void deleteFileIfExists();

private:
    OutputFile outputFile;
    BufferFile bufferFile;

    bool nandDataFileExist();
    void FillZeroDataToAllAddresses(std::ofstream& nandDataFile);
    void FillChangeDatasToAllAddresses(std::ofstream& nandDataFile,
        const std::vector<LbaEntry>& lbaTable);
    bool SuccessConvertToUINT(LbaEntry* splitDatas, std::string& addrStr,
        std::string& dataStr);

    const uint32_t maxLbaOfDevice;
    const static uint32_t INIT_NAND_DATA = 0;
    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string SEPARATOR = ";";

#if BUFFER_REFACTORING
    static const uint32_t NUM_OF_BUFFERS = 5;
    const std::string BUFFER_FOLDER = "./buffer/";
    const std::string EMPTY_BUFFER_FILE_SUFFIX = "_empty";
#endif
};