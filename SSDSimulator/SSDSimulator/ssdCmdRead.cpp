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
        if (line.empty()) continue; // ���� �� ��ŵ

        size_t delimiterPos = line.find(';');
        if (delimiterPos == std::string::npos) continue; // �����ݷ� ������ ��ŵ

        std::string addrStr = line.substr(0, delimiterPos);
        std::string dataStr = line.substr(delimiterPos + 1);

        try {
            uint32_t  addr = std::stoul(addrStr, nullptr, 16);  // 16���� �ּ� ó��
            uint32_t  data = std::stoul(dataStr, nullptr, 16);  // 16���� ������ ó��

            readRawData.push_back({ addr, data });
        }
        catch (const std::exception& e) {
            std::cerr << "�Ľ� ����: " << e.what() << ", line=" << line << std::endl;
            continue; // ������ ������ ��ŵ
        }
    }

    file.close();
}

void SsdReadCmd::readNandData(const std::string& filename) {
    ParseFile(filename);

    // address�� ��ġ�ϴ� �����͸� ã�Ƽ� readData�� ����
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