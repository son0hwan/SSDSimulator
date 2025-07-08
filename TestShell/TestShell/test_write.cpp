#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, WriteJustOnce) {
	EXPECT_CALL(mockSSD, writeToSSD(1, 0x1000000))
		.Times(1)
		.WillRepeatedly(Return(SUCCESS_STRING));

	testShell.fake_command("write 1 0x1000000");
	
	EXPECT_EQ("[Write] Done\n", getPrintedString());
}

TEST_F(ShellFixture, WriteFailureWithWrongAddress) {
	EXPECT_CALL(mockSSD, writeToSSD(100, 0x1000000))
		.Times(1)
		.WillOnce(Return(ERROR_STRING));

	testShell.fake_command("write 100 0x1000000");

	EXPECT_EQ("[Write] Error\n", getPrintedString());
}

TEST_F(ShellFixture, FullWrite) {
	for (int address = 0; address < NUM_OF_LBA; address++) {
		EXPECT_CALL(mockSSD, writeToSSD(address, 0x1000000))
			.Times(1)
			.WillOnce(Return(SUCCESS_STRING));
	}

	testShell.fake_command("fullwrite 0x1000000");

	EXPECT_EQ("[FullWrite] Done\n", getPrintedString());
}