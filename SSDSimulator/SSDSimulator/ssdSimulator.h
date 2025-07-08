#pragma once
#include "IOManager.h"

class SsdSimulator {
public:
    static SsdSimulator& getInstance() {
        static SsdSimulator instance;
        return instance;
    }

    IOManager& getIoManager() {
        return ioManager;
    }

    void init() {
        ioManager.CheckAndCreateNandDataFile();
    }

    void write(uint32_t address, uint32_t value) {
        // Temporary code to pass UT; will be gone once parser code is in place
        if (!CheckAddressRange(address)) return;

        LoadAllDatasFromNand();
        WriteDataToSpecificAddress(address, value);
        ioManager.ProgramAllDatasToNand(readRawData);
        ioManager.updateOutputWriteSuccess();
    }
    
    void erase(uint32_t startAddress, uint32_t eraseSize) {
        // Temporary code to pass UT; will be gone once parser code is in place
        uint32_t endAddress = startAddress + eraseSize - 1;
        if (!CheckAddressRange(endAddress)) return;
        if (startAddress < 1) {
            ioManager.updateOutputError();
            return;
        }

        LoadAllDatasFromNand();

        for (uint32_t lba = startAddress; lba <= endAddress; lba++) {
            WriteDataToSpecificAddress(lba, ZERO);
        }
        ioManager.ProgramAllDatasToNand(readRawData);
        ioManager.updateOutputWriteSuccess();
    }

    uint32_t read(uint32_t address) {
        // Temporary code to pass UT; will be gone once parser code is in place
        if (!CheckAddressRange(address)) return READ_ERROR;

        LoadAllDatasFromNand();
        uint32_t readData = ReadSpecificAddressData(address);
        ioManager.updateOutputReadSuccess(readData);

        return readData;
    }

#ifdef _DEBUG
    uint32_t getMaxSector() {
        return DEFAULT_MAX_LBA_OF_DEVICE;
    }
#endif

private:
    SsdSimulator() {};
    SsdSimulator(const SsdSimulator&) = delete;
    SsdSimulator& operator=(const SsdSimulator&) = delete;

    bool CheckAddressRange(uint32_t address) {
        if (address > DEFAULT_MAX_LBA_OF_DEVICE) {
            ioManager.updateOutputError();
            return false;
        }
        return true;
    }

    void ClearBuffer() {
        if (!readRawData.empty()) readRawData.clear();
    }

    void LoadAllDatasFromNand() {
        ClearBuffer();
        ioManager.CheckAndCreateNandDataFile();
        ioManager.ReadAllDatasToInternalBuffer(readRawData);
    }

    void WriteDataToSpecificAddress(uint32_t address, uint32_t data) {
        readRawData[address].data = data;
    }

    uint32_t ReadSpecificAddressData(uint32_t address) const {
        return readRawData[address].data;
    }

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    const static uint32_t READ_ERROR = 0xDEADBEEF;
    const static uint32_t ZERO = 0x00000000;

    std::vector<ReadRawData> readRawData;
    IOManager ioManager;
};