#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"
#include "common.h"

using namespace testing;

TEST_F(ShellFixture, FlushOnce) {
	EXPECT_CALL(mockSSD, flushToSSD())
		.Times(1)
		.WillRepeatedly(Return(SUCCESS_STRING));

	testShell.fake_command("flush");

	EXPECT_EQ("[Flush] Done\n\n", getPrintedString());
}