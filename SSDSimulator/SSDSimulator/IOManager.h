#pragma once
#include "outputFile.h"
#include "bufferFile.h"
#include "nandFile.h"

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : nandFile(maxLbaOfDevice) {}

    OutputFile& output() { return outputFile; }
    void updateOutputError();
    void updateOutputWriteSuccess();
    void updateOutputReadSuccess(uint32_t readData);

    BufferFile& buffer() { return bufferFile; }
    bool forceCreateFiveFreshBufferFiles();
    bool updateBufferFiles(const std::vector<std::string> buffers);
    std::vector<std::string> getBufferFileList();

    NandFile& nand() { return nandFile; }
    void CheckAndCreateNandDataFile();
    void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);
    void ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable);
    bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
    void CreateNewTempNandFileAndInitForTest();
    void deleteFileIfExists();

private:
    OutputFile outputFile;
    BufferFile bufferFile;
    NandFile nandFile;
};