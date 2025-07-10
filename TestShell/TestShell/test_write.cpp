#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"

using namespace testing;

void writeToOutputFile(std::string contents) {
	std::ofstream file(OUTPUT_FILE_NAME);

	if (!file.is_open()) {
		std::cerr << "fail to open file";
		return;
	}
	file << contents;
	file.close();
}

TEST_F(ShellFixture, WriteJustOnce) {
	EXPECT_CALL(mockSSD, writeToSSD(1, testData))
		.Times(1)
		.WillRepeatedly(Return(SUCCESS_STRING));

	writeToOutputFile("");
	command("write 1 " + testDataStr);
	
	EXPECT_EQ("[Write] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, WriteFailureWithWrongAddress) {
	EXPECT_CALL(mockSSD, writeToSSD(100, testData))
		.Times(1)
		.WillOnce(Return(ERROR_STRING));

	command("write 100 " + testDataStr);

	EXPECT_EQ("[Write] Error\n\n", getPrintedString());
}

TEST_F(ShellFixture, FullWrite) {
	for_each_addr(addr) {
		EXPECT_CALL(mockSSD, writeToSSD(addr, testData))
			.Times(1)
			.WillOnce(Return(SUCCESS_STRING));
	}

	command("fullwrite " + testDataStr);

	EXPECT_EQ("[Full Write] Done\n\n", getPrintedString());
}