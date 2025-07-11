#pragma once
#include "fileIo.h"

class BufferFile {
public:
    BufferFile() = default;

    bool forceCreateFiveFreshBufferFiles() {
        createEmptyBufferFolder();
        return createFiveFreshBufferFiles();
    }

    bool updateBufferFiles(const std::vector<std::string> buffers) {
        createEmptyBufferFolder();
        return createFiveBufferFiles(buffers);
    }

    std::vector<std::string> getBufferFileList() {
        if (!bufferFolderAndFilesExist()) forceCreateFiveFreshBufferFiles();

        std::vector<std::string> buffers;
        for (const auto& fileName : std::filesystem::directory_iterator(BUFFER_FOLDER)) {
            if (fileName.is_regular_file()) {
                buffers.push_back(removeFolderPathFrom(fileName));
            }
        }
        return buffers;
    }

private:
    static const uint32_t NUM_OF_BUFFERS = 5;
    const std::string BUFFER_FOLDER = "./buffer/";
    const std::string EMPTY_BUFFER_FILE_SUFFIX = "_empty";

    bool createFiveBufferFiles(const std::vector<std::string> buffers) {
        for (auto bufferFileName : buffers) {
            if (!createBufferFile(BUFFER_FOLDER + bufferFileName)) return false;
        }
        return true;
    }

    bool createFiveFreshBufferFiles() {
        for (unsigned int buffer = 1; buffer <= NUM_OF_BUFFERS; buffer++) {
            std::string fileName = std::to_string(buffer) + EMPTY_BUFFER_FILE_SUFFIX;
            if (!createBufferFile(BUFFER_FOLDER + fileName)) return false;
        }
        return true;
    }

    void createEmptyBufferFolder() {
        createBufferFolder();
        removeAllFilesInFolder();
    }

    void createBufferFolder() {
        std::filesystem::create_directory(BUFFER_FOLDER);
    }

    bool removeAllFilesInFolder() {
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

    bool createBufferFile(const std::string & fileName) {
        std::ofstream file(fileName, std::ios::trunc);  // trunc: overwrite if exists
        return file.is_open();
    }

    std::string removeFolderPathFrom(const std::filesystem::directory_entry & file) {
        return file.path().filename().string();
    }

    bool fileExists(const std::string fileName) {
        fs::path filePath = fs::path(BUFFER_FOLDER) / fileName;
        if (!fs::exists(filePath)) {
            std::cerr << "File not found: " << filePath << std::endl;
            return false;
        }
        return true;
    }

    bool bufferFolderAndFilesExist() {
        if (!std::filesystem::exists(BUFFER_FOLDER)) return false;        
        
        int numOfFiles = 0;
        for (const auto& fileName : std::filesystem::directory_iterator(BUFFER_FOLDER)) {
            if (fileName.is_regular_file()) numOfFiles++;
        }
        if (numOfFiles != NUM_OF_BUFFERS) return false;

        return true;
    }
};
