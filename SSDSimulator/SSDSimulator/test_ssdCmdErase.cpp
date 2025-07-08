#include "gmock/gmock.h"
#include "ssdCmdErase.h"
#include "ssdCmdRead.h"
#include <vector>

using namespace testing;

class EraseTestFixture : public testing::Test {
public:
	SsdEraseCmd& eraseCmd = SsdEraseCmd::getInstance();
	SsdReadCmd& readCmd = SsdReadCmd::getInstance();

	void erase(uint32_t address, uint32_t size) {
		eraseCmd.setStartAddress(address);
		eraseCmd.setEraseSize(size);
		eraseCmd.run();
	}

	void read(uint32_t address) {
		readCmd.setAddress(address);
		readCmd.run();
	}

	void verifyEraseAndRead(uint32_t address, uint32_t size) {
		EXPECT_NO_THROW(erase(address, size));
		std::vector<uint32_t> datas{};

		for (uint32_t lba = address; lba <= address + size - 1; lba++) {
			datas.push_back(readCmd.getReadData());
		}
		EXPECT_NO_THROW(read(address));
		EXPECT_EQ(datas, expectDatas);
		for (int i = 0; i < 10; i++) {
			std::cout << datas[i] << "\n";
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
	IOManager ioManager;
	std::vector<uint32_t> expectDatas{0,0,0,0,0,0,0,0,0,0};

	static const uint32_t VALID_ADDRESS = 19;
	static const uint32_t INVALID_ADDRESS100 = 100;
	static const uint32_t INVALID_ADDRESS0 = 0;
	static const uint32_t ERASE_SIZE = 10;
	const std::string OUTPUT_ERROR = "ERROR";
	const std::string OUTPUT_FILENAME = "ssd_output.txt";
};

TEST_F(EraseTestFixture, EraseExecutedWithoutError) {
	EXPECT_NO_THROW(erase(VALID_ADDRESS, ERASE_SIZE));
}

TEST_F(EraseTestFixture, EraseExecutedWithErrorInvalidAddress) {
	erase(INVALID_ADDRESS100, ERASE_SIZE);
	CheckOutputFileValid(OUTPUT_ERROR);
	erase(INVALID_ADDRESS0, ERASE_SIZE);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(EraseTestFixture, EraseDataIntegrity) {
	ioManager.deleteFileIfExists();
	ioManager.CreateNewTempNandFileAndInitForTest();
	verifyEraseAndRead(VALID_ADDRESS, ERASE_SIZE);
}
