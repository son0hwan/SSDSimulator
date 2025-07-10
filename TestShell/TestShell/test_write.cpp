#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, WriteJustOnce) {
	EXPECT_CALL(mockSSD, writeToSSD(1, testData))
		.Times(1)
		.WillRepeatedly(Return(SUCCESS_STRING));

	testShell.fake_command("write 1 " + testDataStr);
	
	EXPECT_EQ("[Write] Done\n", getPrintedString());
}

TEST_F(ShellFixture, WriteFailureWithWrongAddress) {
	EXPECT_CALL(mockSSD, writeToSSD(100, testData))
		.Times(1)
		.WillOnce(Return(ERROR_STRING));

	testShell.fake_command("write 100 " + testDataStr);

	EXPECT_EQ("[Write] Error\n", getPrintedString());
}

TEST_F(ShellFixture, FullWrite) {
	for_each_addr(addr) {
		EXPECT_CALL(mockSSD, writeToSSD(addr, testData))
			.Times(1)
			.WillOnce(Return(SUCCESS_STRING));
	}

	testShell.fake_command("fullwrite " + testDataStr);

	EXPECT_EQ("[FullWrite] Done\n", getPrintedString());
}