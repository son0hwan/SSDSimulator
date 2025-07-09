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
        if (!CheckAddressRange(address)) return;

        LoadAllDatasFromNand();
        WriteDataToSpecificAddress(address, value);
        ioManager.ProgramAllDatasToNand(lbaTable);
        ioManager.updateOutputWriteSuccess();
    }

    uint32_t read(uint32_t address) {
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

    void ClearInternalLbaTable() {
        if (!lbaTable.empty()) lbaTable.clear();
    }

    void LoadAllDatasFromNand() {
        ClearInternalLbaTable();
        ioManager.CheckAndCreateNandDataFile();
        ioManager.ReadAllDatasToInternalBuffer(lbaTable);
    }

    void WriteDataToSpecificAddress(uint32_t address, uint32_t data) {
        lbaTable[address].data = data;
    }

    uint32_t ReadSpecificAddressData(uint32_t address) const {
        return lbaTable[address].data;
    }

    const static uint32_t DEFAULT_MAX_LBA_OF_DEVICE = 99;
    const static uint32_t READ_ERROR = 0xDEADBEEF;

    std::vector<LbaEntry> lbaTable;
    IOManager ioManager;
};