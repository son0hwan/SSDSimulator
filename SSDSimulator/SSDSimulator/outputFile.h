#pragma once
#include "fileIo.h"
#include <string>
#include <iostream>
#include <direct.h>  // _getcwd

class OutputFile {
public:
    OutputFile() = default;

    void updateError() {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << OUTPUT_ERROR;
    }

    void updateWriteSuccess() {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << OUTPUT_WRITE_SUCCESS;
    }

    void updateWriteStatus(int writeCnt, bool firstWrite) {
        std::ofstream outputFile;
        if (firstWrite) {
            // 첫 쓰기 때는 덮어쓰기 모드
            outputFile.open(OUTPUT_FILE, std::ios::out);
            if (!outputFile) {
                throw std::runtime_error("error opening file");
            }
            outputFile << OUTPUT_WRITE_SUCCESS;
        }
        else {
            // 그 다음부터는 append 모드
            outputFile.open(OUTPUT_FILE, std::ios::app);
            if (!outputFile) {
                throw std::runtime_error("error opening file");
            }
            outputFile << " " << writeCnt << "\n";
        }
    }

    void updateEraseStatus(int eraseCnt) {
        std::ofstream outputFile(OUTPUT_FILE, std::ios::out);  // truncate 모드 (기본)
        if (!outputFile.is_open()) {
            throw std::runtime_error("Failed to open OUTPUT_FILE for writing status");
        }

        outputFile << OUTPUT_WRITE_SUCCESS << '\n' << eraseCnt;
    }

    void updateReadSuccess(uint32_t readData) {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << "0x" << std::uppercase << std::hex << std::setw(8)
            << std::setfill('0') << readData << std::endl;
    }

    void updateEraseSuccess() {
        this->updateWriteSuccess();
    }

private:
    const std::string OUTPUT_FILE = "C:\Users\User\source\repos\SSDSimulator\SSDSimulator\x64\Release\ssd_output.txt";
    const std::string OUTPUT_ERROR = "ERROR";
    const std::string OUTPUT_WRITE_SUCCESS = "";

    std::ofstream openFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("error opening file");
        }
        return file;
    }

};