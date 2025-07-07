#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"

using namespace testing;

TEST(ShellWrite, Write) {
	MockSSD mock;
	TestShell shell(&mock);
	
	EXPECT_CALL(mock, writeToSSD(1, 0x1000000))
		.Times(1)
		.WillRepeatedly(Return(""));

	int result = shell.write(1, 0x1000000);
	EXPECT_EQ(0, result);
}