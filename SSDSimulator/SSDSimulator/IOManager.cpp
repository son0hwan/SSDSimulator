#include "IOManager.h"

void IOManager::CheckAndCreateNandDataFile() {
    nand().CheckAndCreateNandDataFile();
}

void IOManager::ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable) {
    nand().ProgramAllDatasToNand(lbaTable);
}

void IOManager::ReadAllDatasToInternalLbaTable(std::vector<LbaEntry>& lbaTable) {
    nand().ReadAllDatasToInternalLbaTable(lbaTable);
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
