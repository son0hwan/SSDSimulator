#pragma once
#include "IOManager.h"

class SsdSimulator {
public:
    static SsdSimulator& getInstance() {
        static SsdSimulator instance;
        return instance;
    }

    IOManager& getIoManager();

    void init();

    uint32_t read(uint32_t address);

    void write(uint32_t address, uint32_t value);

    void erase(uint32_t startAddress, uint32_t eraseSize);

    uint32_t getMaxSector();

private:
    SsdSimulator() {};
    SsdSimulator(const SsdSimulator&) = delete;
    SsdSimulator& operator=(const SsdSimulator&) = delete;

    bool CheckAddressRange(uint32_t address);
    bool CheckEraseSize(uint32_t eraseSize);

    void ClearInternalLbaTable();
    void LoadAllDatasFromNand();

    void WriteDataToSpecificAddress(uint32_t address, uint32_t data);
    uint32_t ReadSpecificAddressData(uint32_t address) const;

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    const static uint32_t READ_ERROR = 0xDEADBEEF;
    const static uint32_t ZERO = 0x00000000;
    const static uint32_t MIN_NUM_OF_LBA_TO_ERASE = 1;
    const static uint32_t MAX_NUM_OF_LBA_TO_ERASE = 10;

    std::vector<LbaEntry> lbaTable;
    IOManager ioManager{ DEFAULT_MAX_LBA_OF_DEVICE };
};