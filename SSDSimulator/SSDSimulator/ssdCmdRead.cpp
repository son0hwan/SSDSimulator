#include "ssdCmdRead.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

void SsdReadCmd::CheckAddressRange(long newAddress)
{
    if (newAddress < 0 || newAddress > 99) {
        std::ofstream outFile("ssd_output.txt");
        outFile << "ERROR" << std::endl;
        return;
    }
}

void SsdReadCmd::ParseFile(const std::string& filename) {
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

void SsdReadCmd::readNandData(const std::string& filename) {
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
        readData = foundReadData->data;
        std::ofstream outFile("ssd_output.txt");
        outFile << "0x" << std::hex << std::uppercase << readData << std::endl;
        return;
    }
    else {
        std::ofstream outFile("ssd_output.txt");
        outFile << "ERROR" << std::endl;
        return;
    }
}