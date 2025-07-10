#include "gmock/gmock.h"
#include "ssdCmdWrite.h"
#include "ssdCmdRead.h"
#include "test_ssdCmdFixture.h"

using namespace testing;

class WriteTestFixture : public SsdCmdTestFixture {
public:
	SsdWriteCmd* writeCmd = new SsdWriteCmd();

	void write(uint32_t address, uint32_t data) {
		writeCmd->setAddress(address);
		writeCmd->setWriteData(data);
		writeCmd->run();
	}
	
	void verifyWriteAndRead(uint32_t address, uint32_t data) {
		EXPECT_NO_THROW(write(VALID_ADDRESS, WRITE_DATA));
		EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
		EXPECT_EQ(getReadData(), WRITE_DATA);
		CheckOutputFileValid(OUTPUT_VALID_READ);
	}

	void verifyWriteAndReadAll(uint32_t address, uint32_t data) {
		uint32_t maxAddress = SsdSimulator::getInstance().getMaxSector();
		for (uint32_t address = 0; address <= maxAddress; address++) {
			verifyWriteAndRead(address, data);
		}
	}

};

TEST_F(WriteTestFixture, WriteExecutedWithoutError) {
	EXPECT_NO_THROW(write(VALID_ADDRESS, WRITE_DATA));
	CheckOutputFileValid(OUTPUT_WRITE_SUCCESS);
}

TEST_F(WriteTestFixture, WriteExecutedWithErrorInvalidAddress) {
	write(INVALID_ADDRESS, WRITE_DATA);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(WriteTestFixture, WriteDataIntegrity) {
	setNandFileTestVal();
	verifyWriteAndRead(VALID_ADDRESS, WRITE_DATA);
}

TEST_F(WriteTestFixture, WriteDataIntegrityFullCapacity) {
	setNandFileTestVal();
	verifyWriteAndReadAll(VALID_ADDRESS, WRITE_DATA);
}