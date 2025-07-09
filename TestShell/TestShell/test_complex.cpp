#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, FullWriteAndReadCompare) {
		EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(PASS));

	testShell.fake_command("1_FullWriteAndReadCompare");

	EXPECT_EQ("[1_FullWriteAndReadCompare] Done\n", getPrintedString());
}

TEST_F(ShellFixture, PartialLBAWrite) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(PASS));

	testShell.fake_command("2_PartialLBAWrite");

	EXPECT_EQ("[2_PartialLBAWrite] Done\n", getPrintedString());
}

TEST_F(ShellFixture, WriteReadAging) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(PASS));

	testShell.fake_command("3_WriteReadAging");

	EXPECT_EQ("[3_WriteReadAging] Done\n", getPrintedString());
}