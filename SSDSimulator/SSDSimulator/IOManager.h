#pragma once
#include "outputFile.h"
#include "bufferFile.h"
#include "nandFile.h"

class IOManager {
public:
    IOManager(uint32_t maxLbaOfDevice) : nandFile(maxLbaOfDevice) {}

    OutputFile& output() { return outputFile; }
    BufferFile& buffer() { return bufferFile; }
    NandFile& nand() { return nandFile; }

private:
    OutputFile outputFile;
    BufferFile bufferFile;
    NandFile nandFile;
};