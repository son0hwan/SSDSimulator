#include "gmock/gmock.h"
#include "ssdCmdErase.h"
#include "ssdCmdRead.h"
#include "test_ssdCmdFixture.h"
#include <vector>

using namespace testing;

class EraseTestFixture : public SsdCmdTestFixture {
public:
	SsdEraseCmd eraseCmd;

	void erase(uint32_t address, uint32_t size) {
		eraseCmd.setStartAddress(address);
		eraseCmd.setEraseSize(size);
		eraseCmd.run();
	}

	void verifyEraseAndRead(uint32_t address, uint32_t size) {
		EXPECT_NO_THROW(erase(address, size));
		std::vector<uint32_t> datas{};

		for (uint32_t lba = address; lba <= address + size - 1; lba++) {
			datas.push_back(getReadData());
			EXPECT_NO_THROW(runReadTest(address));
			CheckOutputFileValid(EXPECT_DATA);
		}
		EXPECT_EQ(datas, expectDatas);
	}


protected:
	std::vector<uint32_t> expectDatas{ 0,0,0,0,0,0,0,0,0,0 };
	static const uint32_t INVALID_SIZE = 0;
	static const uint32_t ERASE_SIZE = 10;
	const std::string EXPECT_DATA = "0x00000000";
};

TEST_F(EraseTestFixture, EraseExecutedWithoutError) {
	EXPECT_NO_THROW(erase(VALID_ADDRESS, ERASE_SIZE));
}

TEST_F(EraseTestFixture, EraseExecutedWithErrorInvalidAddress) {
	erase(INVALID_ADDRESS, ERASE_SIZE);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(EraseTestFixture, EraseExecutedWithErrorInvalidSize) {
	erase(VALID_ADDRESS, INVALID_SIZE);
	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(EraseTestFixture, EraseDataIntegrity) {
	setNandFileTestVal();
	verifyEraseAndRead(VALID_ADDRESS, ERASE_SIZE);
}