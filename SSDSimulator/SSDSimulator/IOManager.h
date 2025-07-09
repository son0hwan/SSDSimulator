#pragma once
#include "outputFile.h"
#include "bufferFile.h"
#include "nandFile.h"

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice)  {
        nand().setDeviceMaxLba(maxLbaOfDevice);
    }

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

    NandFile& nand() { return nandFile; }
#if NAND_REFACTORING 
    void CheckAndCreateNandDataFile();
    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);
    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable);
    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
    void CreateNewTempNandFileAndInitForTest();
    void deleteFileIfExists();
#endif

private:
    OutputFile outputFile;
    BufferFile bufferFile;
    NandFile nandFile;
};