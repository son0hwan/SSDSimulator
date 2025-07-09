#include "IOManager.h"

void IOManager::updateOutputError() {
    output().updateOutputError();
}

void IOManager::updateOutputWriteSuccess() {
    output().updateOutputWriteSuccess();
}

void IOManager::updateOutputReadSuccess(uint32_t readData) {
    output().updateOutputReadSuccess(readData);
}

void IOManager::updateOutputEraseSuccess() {
    output().updateOutputWriteSuccess();
}

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

bool IOManager::forceCreateFiveFreshBufferFiles() {
    return buffer().forceCreateFiveFreshBufferFiles();
}

bool IOManager::updateBufferFiles(const std::vector<std::string> buffers) {
    return buffer().updateBufferFiles(buffers);
}

std::vector<std::string> IOManager::getBufferFileList() {
    return buffer().getBufferFileList();
}



