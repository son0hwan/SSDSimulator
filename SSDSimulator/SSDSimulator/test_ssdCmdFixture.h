#pragma once
#include "gmock//gmock.h"
#include "ssdInterface.h"
#include "ssdCmdIncludes.h"

class SsdCmdTestFixture : public testing::Test {
public:

	void SetUp() override {
		if (std::ifstream(NandFile::NAND_DATA_FILE)) {
			std::remove(NandFile::NAND_DATA_FILE);
		}
		ioManager.nand().CheckAndCreateNandDataFile();

		maxLbaofDevice = SsdSimulator::getInstance().getMaxSector();
	}

	void setNandFileTestVal() {
		std::vector<LbaEntry> expectedData{};
		uint32_t initAddress = 0x705FF427;
		for (uint32_t lba = 0; lba <= maxLbaofDevice; ++lba) {
			expectedData.push_back({ lba, initAddress++ });
		}
		ioManager.nand().ProgramAllDatasToNand(expectedData);
	}


	void CheckOutputFileValid(const std::string& expectResult)
	{
		std::ifstream outFile(OUTPUT_FILENAME);
		ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt file open failed";

		std::string fileContent;
		std::getline(outFile, fileContent);
		EXPECT_EQ(fileContent, expectResult);
	}

	void runReadTest(uint32_t address) {
		readCmd.setAddress(address);
		readCmd.run();
	}

	uint32_t getReadData() {
		return readCmd.getReadData();
	}

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };

	static const uint32_t VALID_ADDRESS = 19;
	static const uint32_t INVALID_ADDRESS = 100;
	static const uint32_t WRITE_DATA = 0x705ff43a;
	static constexpr const char* OUTPUT_INIT_READ = "0x00000000";
	static constexpr const char* OUTPUT_VALID_READ = "0x705FF43A";
	static constexpr const char* OUTPUT_ERROR = "ERROR";
	static constexpr const char* OUTPUT_FILENAME = "ssd_output.txt";
	static constexpr const char* NAND_FILENAME = "ssd_nand.txt";
	static constexpr const char* OUTPUT_WRITE_SUCCESS = "";

	SsdReadCmd readCmd;

	uint32_t maxLbaofDevice;
};