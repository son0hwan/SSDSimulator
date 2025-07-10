#pragma once
#include "fileIo.h"

struct LbaEntry {
	LbaEntry(uint32_t address = 0, uint32_t data = 0) : address(address), data(data) {}
	uint32_t address;
	uint32_t data;
};

class NandFile {
public:
	NandFile(uint32_t maxLbaOfDevice) : maxLbaOfDevice(maxLbaOfDevice) {}

	void CheckAndCreateNandDataFile();

	void ProgramAllDatasToNand(const std::vector<LbaEntry>& lbaTable);

	void ReadAllDatasToInternalLbaTable(std::vector<LbaEntry>& lbaTable);

	static constexpr const char* NAND_DATA_FILE = "ssd_nand.txt";
private:
	uint32_t maxLbaOfDevice;
	const static uint32_t INIT_NAND_DATA = 0;
	const std::string SEPARATOR = ";";

	std::ofstream openFile(const std::string& filename);
	bool nandDataFileExist();
	bool SplitStringToAddressAndData(std::string& line, LbaEntry* splitDatas);
	bool isValidHexaString(const std::string& str);
};