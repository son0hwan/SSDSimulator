#include "mockExecutor.cpp"
#include "shell.cpp"
#include "shellFixture.cpp"
#include "common.h"

using namespace testing;

TEST_F(ShellFixture, ReadJustOnce) {
	std::string EXPECTED_STR = genRandomString(MAX_VAL_LEN);
	EXPECT_CALL(mockSSD, readFromSSD)
		.WillOnce(testing::Return(EXPECTED_STR));

	command("read 50");

	EXPECT_EQ("[Read] LBA 50 : " + EXPECTED_STR + "\n\n", getPrintedString());
}

TEST_F(ShellFixture, FullRead) {
	for_each_addr(addr) {
		EXPECT_CALL(mockSSD, readFromSSD(addr))
			.WillOnce(testing::Return(TEST_SAMPLE_DATA));
	}
	command("fullread");
}