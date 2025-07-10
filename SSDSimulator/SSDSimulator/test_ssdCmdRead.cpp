#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdRead.h"
#include "ssdSimulator.h"
#include "test_ssdCmdFixture.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace testing;

class ReadTestFixture : public SsdCmdTestFixture {
public:
	static const uint32_t EXPECTED_DATA = 0x705ff43a;
	static const uint32_t INIT_READ_DATA = 0x00000000;
	static const uint32_t CACHED_DATA = 0x12345678;
};


TEST_F(ReadTestFixture, ReadExecutedWithoutError) {
	EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
}

TEST_F(ReadTestFixture, ReadExecutedWithError) {
	runReadTest(INVALID_ADDRESS);

	CheckOutputFileValid(OUTPUT_ERROR);
}

TEST_F(ReadTestFixture, ReadValidData) {
	setNandFileTestVal();

	EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
	EXPECT_EQ(readCmd.getReadData(), EXPECTED_DATA);

	CheckOutputFileValid(OUTPUT_VALID_READ);
}

TEST_F(ReadTestFixture, ReadNoInputAutoCreateFileAndInitializeToZero) {
	EXPECT_NO_THROW(runReadTest(VALID_ADDRESS));
	EXPECT_EQ(readCmd.getReadData(), INIT_READ_DATA);
	CheckOutputFileValid(OUTPUT_INIT_READ);
}

TEST_F(ReadTestFixture, ReadFromCachedWithoutNandAccess) {
	SsdCachedReadCmd cachedRead(VALID_ADDRESS, CACHED_DATA);
	cachedRead.run();
	EXPECT_EQ(cachedRead.getReadData(), CACHED_DATA);
}