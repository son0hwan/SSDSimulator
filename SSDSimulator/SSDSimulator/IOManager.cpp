#include "IOManager.h"

void IOManager::CheckAndCreateNandDataFile() {
    if (nandDataFileExist()) return;

    auto nandDataFile = openFile(NAND_DATA_FILE);
    FillZeroDataToAllAddresses(nandDataFile);
}

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

void IOManager::ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable) {
    auto nandDataFile = openFile(NAND_DATA_FILE);
    FillChangeDatasToAllAddresses(nandDataFile, lbaTable);
}

void IOManager::ReadAllDatasToInternalBuffer(std::vector<LbaEntry>& lbaTable) {
    std::ifstream file(NAND_DATA_FILE);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        LbaEntry splitDatas;
        if (false == SplitStringToAddressAndData(line, &splitDatas)) continue;
        lbaTable.push_back({ splitDatas.address, splitDatas.data });
    }
}

bool IOManager::SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas) {
    std::string addrStr, dataStr;
    size_t delimiterPos = line.find(SEPARATOR.c_str());
    if (delimiterPos == std::string::npos) return false;

    addrStr = line.substr(0, delimiterPos);
    dataStr = line.substr(delimiterPos + 1);

    return SuccessConvertToUINT(splitDatas, addrStr, dataStr);
}

void IOManager::CreateNewTempNandFileAndInitForTest() {
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

void IOManager::deleteFileIfExists() {
    if (std::ifstream(NAND_DATA_FILE)) {
        std::remove(NAND_DATA_FILE.c_str());
    }
}

#if BUFFER_REFACTORING 
bool IOManager::forceCreateFiveFreshBufferFiles() {
    //createEmptyBufferFolder();
    //return createFiveFreshBufferFiles();
    return buffer().forceCreateFiveFreshBufferFiles();
}

bool IOManager::updateBufferFiles(const std::vector<std::string> buffers) {
    //createEmptyBufferFolder();
    //return createFiveBufferFiles(buffers);
    return buffer().updateBufferFiles(buffers);
}

std::vector<std::string> IOManager::getBufferFileList() {
    //std::vector<std::string> buffers;
    //for (const auto& fileName : std::filesystem::directory_iterator(BUFFER_FOLDER)) {
    //    if (fileName.is_regular_file()) {
    //        buffers.push_back(removeFolderPathFrom(fileName));
    //    }
    //}
    //return buffers;
    return buffer().getBufferFileList();
}
#endif

bool IOManager::nandDataFileExist() {
    std::ifstream file(NAND_DATA_FILE);
    if (!file) return false;
    return true;
}

void IOManager::FillZeroDataToAllAddresses(std::ofstream& nandDataFile) {
    for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
        nandDataFile << std::hex << lba;
        nandDataFile << SEPARATOR;
        nandDataFile << std::setw(8) << std::setfill('0') << INIT_NAND_DATA;
        nandDataFile << std::endl;
    }
}

void IOManager::FillChangeDatasToAllAddresses(std::ofstream& nandDataFile,
    const std::vector<LbaEntry>& lbaTable) {
    for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
        nandDataFile << std::hex << lba;
        nandDataFile << SEPARATOR;
        nandDataFile << std::hex << std::setw(8) << std::setfill('0')
            << lbaTable[lba].data;
        nandDataFile << std::endl;
    }
}

bool IOManager::SuccessConvertToUINT(LbaEntry* splitDatas, std::string& addrStr,
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

