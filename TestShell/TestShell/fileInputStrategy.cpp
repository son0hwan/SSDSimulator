#include "fileInputStrategy.h"
#include <fstream>
#include <iostream>
#include <iomanip>

FileInputStrategy::FileInputStrategy(const std::string& filename) : file(filename) {}

bool FileInputStrategy::hasNextCommand() {
    return file && !file.eof();
}
std::string FileInputStrategy::getNextCommand() {
    std::string cmd;
    std::getline(file, cmd);
    return cmd;
}

void FileInputStrategy::print(const std::string& cmdName, int status) {
    std::string statusStr;

    if (status == SUCCESS)
        statusStr = "Pass";
    else
        statusStr = "FAIL!";
    std::cout << statusStr << std::endl;
}
void FileInputStrategy::prePrint(const std::string& cmdName) {
    std::cout << std::left << std::setfill(' ') << std::setw(30) << cmdName << "___   Run...";
}
