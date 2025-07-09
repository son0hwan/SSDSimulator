#include "IOManager.h"

void IOManager::CheckAndCreateNandDataFile() {
    if (nandDataFileExist()) return;

    auto nandDataFile = openFile(NAND_DATA_FILE);
    FillZeroDataToAllAddresses(nandDataFile);
}

void IOManager::updateOutputError() {
    auto outputFile = openFile(OUTPUT_FILE);
    outputFile << OUTPUT_ERROR;
}

void IOManager::updateOutputWriteSuccess() {
    auto outputFile = openFile(OUTPUT_FILE);
}

void IOManager::updateOutputReadSuccess(uint32_t readData) {
    auto outputFile = openFile(OUTPUT_FILE);
    outputFile << "0x" << std::uppercase << std::hex << std::setw(8)
        << std::setfill('0') << readData << std::endl;
}

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

bool IOManager::forceCreateFiveFreshBufferFiles() {
    createEmptyBufferFolder();
    return createFiveFreshBufferFiles();
}

bool IOManager::updateBufferFiles(const std::vector<std::string> buffers) {
    createEmptyBufferFolder();
    return createFiveBufferFiles(buffers);
}

std::vector<std::string> IOManager::getBufferFileList() {
    std::vector<std::string> buffers;
    for (const auto& fileName : std::filesystem::directory_iterator(BUFFER_FOLDER)) {
        if (fileName.is_regular_file()) {
            buffers.push_back(removeFolderPathFrom(fileName));
        }
    }
    return buffers;
}

bool IOManager::nandDataFileExist() {
    std::ifstream file(NAND_DATA_FILE);
    if (!file) return false;
    return true;
}

std::ofstream IOManager::openFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("error opening file");
    }
    return file;
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

bool IOManager::createFiveBufferFiles(const std::vector<std::string> buffers) {
    for (auto bufferFileName : buffers) {
        if (!createBufferFile(BUFFER_FOLDER + bufferFileName)) return false;
    }
    return true;
}

bool IOManager::createFiveFreshBufferFiles() {
    for (unsigned int buffer = 1; buffer <= NUM_OF_BUFFERS; buffer++) {
        std::string fileName = std::to_string(buffer) + EMPTY_BUFFER_FILE_SUFFIX;
        if (!createBufferFile(BUFFER_FOLDER + fileName)) return false;
    }
    return true;
}

void IOManager::createEmptyBufferFolder() {
    createBufferFolder();
    removeAllFilesInFolder();
}

void IOManager::createBufferFolder() {
    std::filesystem::create_directory(BUFFER_FOLDER);
}

bool IOManager::removeAllFilesInFolder() {
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

bool IOManager::createBufferFile(const std::string& fileName) {
    std::ofstream file(fileName, std::ios::trunc);  // trunc: overwrite if exists
    return file.is_open();
}

std::string IOManager::removeFolderPathFrom(const std::filesystem::directory_entry& file) {
    return file.path().filename().string();
}

bool IOManager::fileExists(const std::string fileName) {
    fs::path filePath = fs::path(BUFFER_FOLDER) / fileName;
    if (!fs::exists(filePath)) {
        std::cerr << "File not found: " << filePath << std::endl;
        return false;
    }
    return true;
}