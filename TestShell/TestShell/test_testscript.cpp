#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"

using namespace testing;

TEST_F(ShellFixture, FullWriteAndReadCompare) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(TEST_SAMPLE_DATA));
	EXPECT_CALL(mockRandomGenerator, next)
		.WillRepeatedly(Return(std::stoul(TEST_SAMPLE_DATA, nullptr, 16)));

	command("1_FullWriteAndReadCompare");

	EXPECT_EQ("[1_FullWriteAndReadCompare] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, PartialLBAWrite) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(TEST_SAMPLE_DATA));
	EXPECT_CALL(mockRandomGenerator, next)
		.WillRepeatedly(Return(std::stoul(TEST_SAMPLE_DATA, nullptr, 16)));

	command("2_PartialLBAWrite");

	EXPECT_EQ("[2_PartialLBAWrite] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, WriteReadAging) {
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, readFromSSD(_))
		.WillRepeatedly(Return(TEST_SAMPLE_DATA));
	EXPECT_CALL(mockRandomGenerator, next)
		.WillRepeatedly(Return(std::stoul(TEST_SAMPLE_DATA, nullptr, 16)));

	command("3_WriteReadAging");

	EXPECT_EQ("[3_WriteReadAging] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, EraseAndWriteAging) {
	int toggle = 0;

	EXPECT_CALL(mockSSD, eraseToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, writeToSSD(_, _))
		.WillRepeatedly(Return(SUCCESS_STRING));
	EXPECT_CALL(mockRandomGenerator, next)
		.WillRepeatedly([&toggle]() {
		if (toggle == 0) {
			toggle = 1;
			return std::stoul(TEST_SAMPLE_DATA, nullptr, 16);
		}
		else {
			toggle = 0;
			return std::stoul(TEST_SAMPLE_DATA2, nullptr, 16);
		}
			});

	command("4_EraseAndWriteAging");

	EXPECT_EQ("[4_EraseAndWriteAging] Done\n\n", getPrintedString());
}