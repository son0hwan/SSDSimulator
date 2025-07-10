#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"
#include "common.h"

using namespace testing;

TEST_F(ShellFixture, Erase10Blocks) {
	EXPECT_CALL(mockSSD, eraseToSSD(5, 10))
		.Times(1)
		.WillRepeatedly(Return(SUCCESS_STRING));

	command("erase 5 10");

	EXPECT_EQ("[Erase] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, EraseOutOfRange) {
	EXPECT_CALL(mockSSD, eraseToSSD(95, 10))
		.Times(1)
		.WillOnce(Return(ERROR_STRING));

	command("erase 95 10");

	EXPECT_EQ("[Erase] Error\n\n", getPrintedString());
}

TEST_F(ShellFixture, RangedErase) {
	EXPECT_CALL(mockSSD, eraseToSSD(15, 10))
		.Times(1)
		.WillOnce(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, eraseToSSD(25, 6))
		.Times(1)
		.WillOnce(Return(SUCCESS_STRING));

	command("erase_range 15 30");

	EXPECT_EQ("[RangedErase] Done\n\n", getPrintedString());
}

TEST_F(ShellFixture, RangedEraseOutOfRange) {
	EXPECT_CALL(mockSSD, eraseToSSD(75, 10))
		.Times(1)
		.WillOnce(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, eraseToSSD(85, 10))
		.Times(1)
		.WillOnce(Return(SUCCESS_STRING));
	EXPECT_CALL(mockSSD, eraseToSSD(95, 10))
		.Times(1)
		.WillOnce(Return(ERROR_STRING));

	command("erase_range 75 105");

	EXPECT_EQ("[RangedErase] Error\n\n", getPrintedString());
}