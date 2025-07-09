#include "IOManager.h"

#if OUTPUT_REFACTORING
void IOManager::updateOutputError() {
    output().updateOutputError();
}

void IOManager::updateOutputWriteSuccess() {
    output().updateOutputWriteSuccess();
}

void IOManager::updateOutputReadSuccess(uint32_t readData) {
    output().updateOutputReadSuccess(readData);
}

std::ofstream IOManager::openFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("error opening file");
    }
    return file;
}
#endif

#if NAND_REFACTORING 
void IOManager::CheckAndCreateNandDataFile() {
    nand().CheckAndCreateNandDataFile();
}

void IOManager::ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable) {
    nand().ProgramAllDatasToNand(lbaTable);
}

void IOManager::ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable) {
    nand().ReadAllDatasToInternalBuffer(lbaTable);
}

bool IOManager::SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas) {
    return nand().SplitStringToAddressAndData(line, splitDatas);
}

void IOManager::CreateNewTempNandFileAndInitForTest() {
    nand().CreateNewTempNandFileAndInitForTest();
}

void IOManager::deleteFileIfExists() {
    nand().deleteFileIfExists();
}
#endif

#if BUFFER_REFACTORING 
bool IOManager::forceCreateFiveFreshBufferFiles() {
    return buffer().forceCreateFiveFreshBufferFiles();
}

bool IOManager::updateBufferFiles(const std::vector<std::string> buffers) {
    return buffer().updateBufferFiles(buffers);
}

std::vector<std::string> IOManager::getBufferFileList() {
    return buffer().getBufferFileList();
}
#endif



