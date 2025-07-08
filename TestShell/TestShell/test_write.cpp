#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, Write) {
	EXPECT_CALL(mock, writeToSSD(1, 0x1000000))
		.Times(1)
		.WillRepeatedly(Return(WRITE_SUCCESS_STRING));

	int result = shell.write(1, 0x1000000);
	EXPECT_EQ(WRITE_SUCCESS, result);
}

TEST_F(ShellFixture, WriteFailureWithWrongAddress) {
	EXPECT_CALL(mock, writeToSSD(100, 0x1000000))
		.Times(1)
		.WillOnce(Return(WRITE_ERROR_STRING));

	int result = shell.write(100, 0x1000000);
	EXPECT_EQ(WRITE_ERROR, result);
}

TEST_F(ShellFixture, FullWrite) {
	for (int address = 0; address < 100; address++) {
		EXPECT_CALL(mock, writeToSSD(address, 0x1000000))
			.Times(1)
			.WillOnce(Return(WRITE_SUCCESS_STRING));
	}

	int result = shell.fullwrite(0x1000000);

	EXPECT_EQ(WRITE_SUCCESS, result);
}