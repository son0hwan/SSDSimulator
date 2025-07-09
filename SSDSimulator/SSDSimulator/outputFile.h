#pragma once
#include "fileIo.h"

class OutputFile {
public:
    OutputFile() = default;

    void updateOutputError() {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << OUTPUT_ERROR;
    }

    void updateOutputWriteSuccess() {
        auto outputFile = openFile(OUTPUT_FILE);
    }

    void updateOutputReadSuccess(uint32_t readData) {
        auto outputFile = openFile(OUTPUT_FILE);
        outputFile << "0x" << std::uppercase << std::hex << std::setw(8)
            << std::setfill('0') << readData << std::endl;
    }

private:
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string OUTPUT_ERROR = "ERROR";

    std::ofstream openFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("error opening file");
        }
        return file;
    }

};