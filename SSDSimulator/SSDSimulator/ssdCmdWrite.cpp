#include "ssdCmdWrite.h"

void SsdWriteCmd::run() {
    if (!CheckAddressRange(requestedAddress))return;
    readNandData(NAND_DATA_FILE);
    updateDataInInternalBuffer(this->requestedAddress, this->data);
    updateNandData();
    updateOutput();
}

bool SsdWriteCmd::CheckAddressRange(uint32_t address) {
    if (address < 0 || address > DEVICE_MAX_ADDRESS) {
        updateOutputError();
        return false;
    }
    return true;
}

void SsdWriteCmd::readNandData(const std::string& filename) {
    if (!readRawData.empty()) readRawData.clear();
    
    ParseFile(filename);

    // address와 일치하는 데이터를 찾아서 readData에 저장
    auto foundReadData = std::find_if(
        readRawData.begin(),
        readRawData.end(),
        [this](const ReadRawData& readEntry) {
            return readEntry.address == requestedAddress;
        }
    );

    if (foundReadData != readRawData.end()) {
        data = foundReadData->data;
        std::ofstream outFile("ssd_output.txt");
        outFile << "0x" << std::hex << std::uppercase << data << std::endl;
        return;
    }
    else {
        std::ofstream outFile("ssd_output.txt");
        outFile << "ERROR" << std::endl;
        return;
    }
}

void SsdWriteCmd::updateDataInInternalBuffer(uint32_t address, uint32_t data) {
    readRawData[this->requestedAddress].data = this->data;
}

void SsdWriteCmd::updateNandData()
{
    std::ofstream nandDataFile(NAND_DATA_FILE); // Open file for writing
    if (!nandDataFile) {
        throw std::exception("error opening file for writing");
    }

    for (int addr = 0; addr < readRawData.size(); addr++) {
        WriteSectorAddressAndDataToNAND(nandDataFile, addr);
    }
    nandDataFile.close();
}

void SsdWriteCmd::updateOutput()
{
    std::ofstream outputFile(OUTPUT_FILE);
    if (!outputFile) {
        throw std::exception("error opening file for writing");
    }
    outputFile.close();
}

void SsdWriteCmd::updateOutputError() {
    std::ofstream outputFile(OUTPUT_FILE);
    if (!outputFile) {
        throw std::exception("error opening file for writing");
    }
    outputFile << OUTPUT_ERROR;
    outputFile.close();
}

void SsdWriteCmd::WriteSectorAddressAndDataToNAND(std::ofstream& nandDataFile, uint32_t addr)
{
    nandDataFile << std::hex << addr; 
    nandDataFile << SEPARATOR;
    nandDataFile << std::hex << std::setw(8) << std::setfill('0') << readRawData[addr].data << std::endl;
}

void SsdWriteCmd::setAddress(uint32_t newAddress) {
    CheckAddressRange(newAddress);
    requestedAddress = newAddress;
}

void SsdWriteCmd::setWriteData(uint32_t newWriteData) {
    data = newWriteData;
}

void SsdWriteCmd::ParseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::exception("error opening file for reading");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue; // 공백 줄 스킵

        size_t delimiterPos = line.find(';');
        if (delimiterPos == std::string::npos) continue; // 세미콜론 없으면 스킵

        std::string addrStr = line.substr(0, delimiterPos);
        std::string dataStr = line.substr(delimiterPos + 1);

        try {
            uint32_t  addr = std::stoul(addrStr, nullptr, 16);  // 16진수 주소 처리
            uint32_t  data = std::stoul(dataStr, nullptr, 16);  // 16진수 데이터 처리

            readRawData.push_back({ addr, data });
        }
        catch (const std::exception& e) {
            std::cerr << "파싱 실패: " << e.what() << ", line=" << line << std::endl;
            continue; // 실패한 라인은 스킵
        }
    }

    file.close();
}
