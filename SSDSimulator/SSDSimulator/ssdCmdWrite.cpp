#include "ssdCmdWrite.h"

void SsdWriteCmd::run() {
    if (this->address > DEVICE_MAX_ADDRESS || 
        this->address < 0) {
        updateOutputError();
        return;
    }

    // Temporary function to mock read from ssd_nand.txt (this will be implemented later on) 
    // The data will be stored to private vector within class
    TEMPORARY_READ_FROM_SSD_NAND_TXT();

    // write to internal vector; the whole vector will be flushed to raw_nand.txt later 
    updateDataInInternalBuffer(this->address, this->data);
    updarteDataInNAND();
    updateOutput();
}

void SsdWriteCmd::updateOutput()
{
    std::ofstream outputFile(OUTPUT_FILE);
    if (!outputFile) {
        throw std::exception("error opening file for writing");
    }
    outputFile.close();
}

void SsdWriteCmd::updateOutputError() {
    std::ofstream outputFile(OUTPUT_FILE);
    if (!outputFile) {
        throw std::exception("error opening file for writing");
    }
    outputFile << OUTPUT_ERROR;
    outputFile.close();
}

void SsdWriteCmd::updarteDataInNAND()
{
    std::ofstream nandDataFile(NAND_DATA_FILE); // Open file for writing
    if (!nandDataFile) {
        throw std::exception("error opening file for writing");
    }

    for (int addr = 0; addr < v.size(); addr++) {
        nandDataFile << std::hex << addr << ";" << std::hex << std::setw(8) << std::setfill('0') << v[addr].data << std::endl;
    }
    nandDataFile.close();
}

void SsdWriteCmd::updateDataInInternalBuffer(long address, long data) {
    v[this->address].data = this->data;
}

#if (1 == TEMPORARY_CODE_FOR_TESTING)    
long SsdWriteCmd::TEMPORARY_READ_SECTOR(long address) {
    if (this->address > DEVICE_MAX_ADDRESS ||
        this->address < 0) {
        throw std::exception("Invalid address range; must be greater than 0 and less than 100");
    }

    return v[address].data;
}

std::vector<std::string> SsdWriteCmd::TEMPORARY_READ_OUTPUT() {
    std::ifstream inFile(OUTPUT_FILE);
    if (!inFile) {
        throw std::exception("error opening file for writing");
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inFile, line)) {
        lines.push_back(line);
    }

    inFile.close();

    return lines;
}

void SsdWriteCmd::TEMPORARY_READ_FROM_SSD_NAND_TXT() {  

    for (int addr = 0; addr <= DEVICE_MAX_ADDRESS; addr++) {
        v.push_back(ReadWriteData{ (long)addr, TEMPORARY_GENERATE_RANDOM_NUMBER()});
    }
}

long SsdWriteCmd::TEMPORARY_GENERATE_RANDOM_NUMBER() {
    std::random_device rd;

    // Create a Mersenne Twister RNG seeded with rd
    std::mt19937_64 gen(rd());

    // Create a uniform distribution for 32-bit unsigned range
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    // Generate a random number
    long random_number = dist(gen);

    return random_number;
}
#endif