#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, WriteReadAging) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(PASS));

	testShell.fake_command("3_WriteReadAging");

	EXPECT_EQ("[3_WriteReadAging] Done\n", getPrintedString());
}