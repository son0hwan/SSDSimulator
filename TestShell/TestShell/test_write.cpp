#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"
#include <vector>
#include <random>

using namespace testing;
using std::vector;

TEST_F(ShellFixture, WriteJustOnce) {
	EXPECT_CALL(mockSSD, writeToSSD(1, 0x1000000))
		.Times(1)
		.WillRepeatedly(Return(WRITE_SUCCESS_STRING));

	int result = testShell.write(1, 0x1000000);
	EXPECT_EQ(WRITE_SUCCESS, result);
}

TEST_F(ShellFixture, WriteFailureWithWrongAddress) {
	EXPECT_CALL(mockSSD, writeToSSD(100, 0x1000000))
		.Times(1)
		.WillOnce(Return(WRITE_ERROR_STRING));

	int result = testShell.write(100, 0x1000000);
	EXPECT_EQ(WRITE_ERROR, result);
}

TEST_F(ShellFixture, FullWrite) {
	for (int address = 0; address < NUM_OF_LBA; address++) {
		EXPECT_CALL(mockSSD, writeToSSD(address, 0x1000000))
			.Times(1)
			.WillOnce(Return(WRITE_SUCCESS_STRING));
	}

	int result = testShell.fullwrite(0x1000000);

	EXPECT_EQ(WRITE_SUCCESS, result);
}
