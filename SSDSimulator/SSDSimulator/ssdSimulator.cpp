#pragma once
#include "IOManager.h"
#include "ssdSimulator.h"

IOManager& SsdSimulator::getIoManager() {
	return ioManager;
}

void SsdSimulator::init() {
	ioManager.nand().CheckAndCreateNandDataFile();
}

uint32_t SsdSimulator::read(uint32_t address) {
	if (!CheckAddressRange(address)) return READ_ERROR;

	LoadAllDatasFromNand();
	uint32_t readData = ReadSpecificAddressData(address);
	ioManager.output().updateReadSuccess(readData);

	return readData;
}

void SsdSimulator::write(uint32_t address, uint32_t value) {
	if (!CheckAddressRange(address)) return;

	LoadAllDatasFromNand();
	WriteDataToSpecificAddress(address, value);
	ioManager.nand().ProgramAllDatasToNand(lbaTable);
	ioManager.output().updateWriteSuccess();
}

void SsdSimulator::erase(uint32_t startAddress, uint32_t eraseSize) {
	uint32_t endAddress = startAddress + eraseSize - 1;
	if (!CheckAddressRange(endAddress)) return;
	if (!CheckEraseSize(eraseSize)) return;
	LoadAllDatasFromNand();

	for (uint32_t lba = startAddress; lba <= endAddress; lba++) {
		WriteDataToSpecificAddress(lba, ZERO);
	}
	ioManager.nand().ProgramAllDatasToNand(lbaTable);
	ioManager.output().updateEraseSuccess();
}

uint32_t SsdSimulator::getMaxSector() {
	return DEFAULT_MAX_LBA_OF_DEVICE;
}

bool SsdSimulator::CheckAddressRange(uint32_t address) {
	if (address > DEFAULT_MAX_LBA_OF_DEVICE) {
		ioManager.output().updateError();
		return false;
	}
	return true;
}

bool SsdSimulator::CheckEraseSize(uint32_t eraseSize) {
	if (eraseSize < MIN_NUM_OF_LBA_TO_ERASE ||
		eraseSize > MAX_NUM_OF_LBA_TO_ERASE) {
		ioManager.output().updateError(); 
		return false;
	}
	return true;
}

void SsdSimulator::ClearInternalLbaTable() {
	if (!lbaTable.empty()) lbaTable.clear();
}

void SsdSimulator::LoadAllDatasFromNand() {
	ClearInternalLbaTable();
	ioManager.nand().CheckAndCreateNandDataFile();
	ioManager.nand().ReadAllDatasToInternalLbaTable(lbaTable);
}

void SsdSimulator::WriteDataToSpecificAddress(uint32_t address, uint32_t data) {
	lbaTable[address].data = data;
}

uint32_t SsdSimulator::ReadSpecificAddressData(uint32_t address) const {
	return lbaTable[address].data;
}