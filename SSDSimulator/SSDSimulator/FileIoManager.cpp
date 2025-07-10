#include "IOManager.h"
#include "nandFile.h"

void NandFile::CheckAndCreateNandDataFile() {
    if (nandDataFileExist()) return;
    auto nandDataFile = openFile(NAND_DATA_FILE);

    std::vector<LbaEntry> emptyLbaTable{};
    for (uint32_t lba = 0; lba <= maxLbaOfDevice; ++lba) {
        emptyLbaTable.emplace_back(lba, 0);
    }
    ProgramAllDatasToNand(emptyLbaTable);
}


void NandFile::ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable) {
    auto nandDataFile = openFile(NAND_DATA_FILE);
    for (const LbaEntry& lbaEntry : lbaTable) {
        nandDataFile << std::hex << lbaEntry.address;
        nandDataFile << SEPARATOR;
        nandDataFile << std::hex << std::setw(8) << std::setfill('0')
            << lbaEntry.data;
        nandDataFile << std::endl;
    }
}

void NandFile::ReadAllDatasToInternalLbaTable(std::vector<LbaEntry>& lbaTable) {
    std::ifstream file(NAND_DATA_FILE);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        LbaEntry splitDatas;
        if (false == SplitStringToAddressAndData(line, &splitDatas)) continue;
        lbaTable.emplace_back(splitDatas.address, splitDatas.data);
    }
}

std::ofstream NandFile::openFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("error opening file");
    }
    return file;
}

bool NandFile::nandDataFileExist() {
    std::ifstream file(NAND_DATA_FILE);
    if (!file) return false;
    return true;
}

bool NandFile::SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas) {
    std::string addrStr, dataStr;
    size_t delimiterPos = line.find(SEPARATOR.c_str());
    if (delimiterPos == std::string::npos) return false;

    addrStr = line.substr(0, delimiterPos);
    dataStr = line.substr(delimiterPos + 1);

    if (false == isValidHexaString(addrStr)) return false;
    if (false == isValidHexaString(dataStr)) return false;

    (*splitDatas).address = std::stoul(addrStr, nullptr, 16);
    (*splitDatas).data = std::stoul(dataStr, nullptr, 16);

    return true;
}

bool NandFile::isValidHexaString(const std::string& str) {
    try {
        std::stoul(str, nullptr, 16);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Fail convert string to unsigned long :";
        std::cerr << e.what() << std::endl;
        return false;
    }
}