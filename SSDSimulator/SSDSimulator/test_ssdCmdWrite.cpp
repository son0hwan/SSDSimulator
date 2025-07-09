#include "gmock/gmock.h"
#include "ssdCmdWrite.h"
#include "ssdCmdRead.h"

using namespace testing;

class WriteTestFixture : public testing::Test {
public:
	SsdWriteCmd& writeCmd = SsdWriteCmd::getInstance();
	SsdReadCmd& readCmd = SsdReadCmd::getInstance();

	void write(uint32_t address, uint32_t data) {
		writeCmd.setAddress(address);
		writeCmd.setWriteData(data);
		writeCmd.run();
	}

	void read(uint32_t address) {
		readCmd.setAddress(address);
		readCmd.run();
	}
	
	void verifyWriteAndRead(uint32_t address, uint32_t data) {
		EXPECT_NO_THROW(write(VALID_ADDRESS, WRITE_DATA));
		EXPECT_NO_THROW(read(VALID_ADDRESS));
		EXPECT_EQ(getReadData(), WRITE_DATA);
		CheckOutputFileValid(OUTPUT_VALID_READ);
	}

	void verifyWriteAndReadAll(uint32_t address, uint32_t data) {
		uint32_t maxAddress = SsdSimulator::getInstance().getMaxSector();
		for (uint32_t address = 0; address <= maxAddress; address++) {
			verifyWriteAndRead(address, data);
		}
	}

	uint32_t getReadData() {
		return readCmd.getReadData();
	}

	void CheckOutputFileValid(const std::string& expectResult)
	{
		std::ifstream outFile(OUTPUT_FILENAME);
		ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt file open failed";

		std::string fileContent;
		std::getline(outFile, fileContent);
		EXPECT_EQ(fileContent, expectResult);
	}

protected:
	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };

	static const uint32_t VALID_ADDRESS = 19;
	static const uint32_t INVALID_ADDRESS = 100;
	static const uint32_t WRITE_DATA = 0x705ff43a;
	const std::string OUTPUT_VALID_READ = "0x705FF43A";
	const std::string OUTPUT_ERROR = "ERROR";
	const std::string OUTPUT_FILENAME = "ssd_output.txt";
};

TEST_F(WriteTestFixture, WriteExecutedWithoutError) {
	EXPECT_NO_THROW(write(VALID_ADDRESS, WRITE_DATA));
}

TEST_F(WriteTestFixture, WriteExecutedWithErrorInvalidAddress) {
	write(INVALID_ADDRESS, WRITE_DATA);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(WriteTestFixture, WriteDataIntegrity) {
	ioManager.deleteFileIfExists();
	ioManager.CreateNewTempNandFileAndInitForTest();
	verifyWriteAndRead(VALID_ADDRESS, WRITE_DATA);
}

TEST_F(WriteTestFixture, WriteDataIntegrityFullCapacity) {
	ioManager.deleteFileIfExists();
	ioManager.CreateNewTempNandFileAndInitForTest();
	verifyWriteAndReadAll(VALID_ADDRESS, WRITE_DATA);
}