#include "fileInputStrategy.h"
#include <fstream>

FileInputStrategy::FileInputStrategy(const std::string& filename) : file(filename) {}

bool FileInputStrategy::hasNextCommand() {
    return file && !file.eof();
}
std::string FileInputStrategy::getNextCommand() {
    std::string cmd;
    std::getline(file, cmd);
    return cmd;
}
