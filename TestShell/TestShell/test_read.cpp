#include <random>
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"

using namespace testing;

std::vector<std::string> generateAlphabet() {
	std::vector<std::string> result;
	const char HEX_DIGITS[] = "0123456789ABCDEF";
	for (char c = 0; c < MAX_HEX_LENGTH; ++c) {
		result.emplace_back(1, HEX_DIGITS[c]);
	}
	return result;
}

TEST_F(ShellFixture, ReadJustOnce) {
	std::vector<std::string> alphabetList = generateAlphabet();

	std::string EXPECTED_STR = "0x";
	for (int i = 0; i < MAX_VAL_LEN; i++)
		EXPECTED_STR.append(alphabetList[rand() % MAX_HEX_LENGTH]);
		
	EXPECT_CALL(mockSSD, readFromSSD)
		.WillOnce(testing::Return(EXPECTED_STR));

	testShell.writeToOutputFile(EXPECTED_STR);
	testShell.fake_command("read 50");

	EXPECT_EQ("[Read] LBA 50 : " + EXPECTED_STR + "\n", getPrintedString());
}

TEST_F(ShellFixture, FullRead) {
	for_each_addr(addr) {
		EXPECT_CALL(mockSSD, readFromSSD(addr))
			.WillOnce(testing::Return(SUCCESS_STRING));
	}

	testShell.fake_command("fullread");
}