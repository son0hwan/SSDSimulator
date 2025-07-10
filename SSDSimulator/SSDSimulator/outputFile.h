#pragma once
#include "fileIo.h"

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

    void updateReadSuccess(uint32_t readData) {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << "0x" << std::uppercase << std::hex << std::setw(8)
            << std::setfill('0') << readData << std::endl;
    }

    void updateEraseSuccess() {
        this->updateWriteSuccess();
    }

private:
    const std::string OUTPUT_FILE = "ssd_output.txt";
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