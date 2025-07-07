#pragma once
#include "ssdInterface.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>

#define TEMPORARY_CODE_FOR_TESTING (1)

// Temporarily defined it here just for testing purpose 
#if (1 == TEMPORARY_CODE_FOR_TESTING)    
struct ReadWriteData {
    long address;
    long data;
};
#endif 

class SsdWriteCmd : public SsdCmdInterface {
public:
    // 생성자는 parser에서 불러줄 것
    SsdWriteCmd(long address, long data) : address(address), data(data) {}

    void run() override;

    void updateOutput();

    void updateOutputError();

    void updateDataToNAND();
    void WriteSectorAddressAndData(std::ofstream& nandDataFile, int addr);
#if (1 == TEMPORARY_CODE_FOR_TESTING)  
    long TEMPORARY_READ_SECTOR_FROM_INTERNAL_BUFFER(long address);

    std::vector<std::string> TEMPORARY_READ_OUTPUT();
    void TEMPORARY_READ_FROM_SSD_NAND_TXT();
    long TEMPORARY_GENERATE_RANDOM_NUMBER();
    std::vector<ReadWriteData> v;
#endif

private:
    const long address;
    const long data;
    static const int DEVICE_MAX_ADDRESS = 99;

    const std::string NAND_DATA_FILE = "ssd_nand.txt";
    const std::string OUTPUT_FILE = "ssd_output.txt";
    const std::string SEPARATOR = ";";
    const std::string OUTPUT_ERROR = "ERROR";

    void updateDataInInternalBuffer(long address, long data);
};