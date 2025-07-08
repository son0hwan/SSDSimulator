#include "gmock/gmock.h"
#include "ssdCmdWrite.h"
#include "ssdCmdRead.h"

using namespace testing;

class WriteTestFixture : public testing::Test {
public:
	SsdWriteCmd& writeCmd = SsdWriteCmd::getInstance();
	SsdReadCmd& readCmd = SsdReadCmd::getInstance();

	void runWriteTest(unsigned long address, unsigned long data) {
		writeCmd.setAddress(address);
		writeCmd.setWriteData(data);
		writeCmd.run();
	}

	void CheckOutputFileValid(const std::string& expectResult)
	{
		std::ifstream outFile(OUTPUT_FILENAME);
		ASSERT_TRUE(outFile.is_open()) << "ssd_output.txt file open failed";

		std::string fileContent;
		std::getline(outFile, fileContent);
		EXPECT_EQ(fileContent, expectResult);
	}

	void runRead(uint32_t address) {
		readCmd.setAddress(address);
		readCmd.run();
	}

protected:
	static const uint32_t DEFAULT_ADDRESS = 0;
	static const uint32_t DEFAULT_WRITE_DATA = 0;
	static const uint32_t VALID_ADDRESS = 19;
	static const uint32_t INVALID_ADDRESS = 100;
	static const uint32_t WRITE_DATA = 0x705ff43a;
	static const uint32_t EXPECTED_DATA = WRITE_DATA;
	static const std::string OUTPUT_ERROR;
	static const std::string OUTPUT_VALID_READ;
	static const std::string OUTPUT_FILENAME;
	static const std::string NAND_FILENAME;
};

const std::string WriteTestFixture::OUTPUT_ERROR = "ERROR";
const std::string WriteTestFixture::OUTPUT_FILENAME = "ssd_output.txt";
const std::string WriteTestFixture::OUTPUT_VALID_READ = "0x705FF43A";
const std::string WriteTestFixture::NAND_FILENAME = "ssd_nand.txt";

TEST_F(WriteTestFixture, WriteExecutedWithoutError) {
	EXPECT_NO_THROW(runWriteTest(VALID_ADDRESS, WRITE_DATA));
}

TEST_F(WriteTestFixture, WriteExecutedWithErrorInvalidAddress) {
	runWriteTest(INVALID_ADDRESS, WRITE_DATA);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(WriteTestFixture, WriteDataIntegrity) {
	EXPECT_NO_THROW(runWriteTest(VALID_ADDRESS, WRITE_DATA));
	EXPECT_NO_THROW(runRead(VALID_ADDRESS));
	EXPECT_EQ(readCmd.getReadData(), WRITE_DATA);

	CheckOutputFileValid(OUTPUT_VALID_READ);
}