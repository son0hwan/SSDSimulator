#include <random>
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"

using namespace testing;

std::vector<std::string> generateAlphabet() {
	std::vector<std::string> result;
	for (char c = 'A'; c <= 'Z'; ++c) {
		result.emplace_back(1, c);
	}
	return result;
}

TEST_F(ShellFixture, ReadJustOnce) {
	std::vector<std::string> alphabetList = generateAlphabet();

	std::string EXPECTED_STR = "0x";
	for (int i = 0; i < MAX_VAL_LEN; i++)
		EXPECTED_STR.append(alphabetList[rand() % MAX_ALPHABET_SIZE]);
		
	EXPECT_CALL(mockSSD, readFromSSD)
		.WillOnce(testing::Return(EXPECTED_STR));

	EXPECT_EQ(EXPECTED_STR, testShell.read(50));
}

TEST_F(ShellFixture, FullRead) {
	for (int i = 0; i < NUM_OF_LBA; i++) {
		EXPECT_CALL(mockSSD, readFromSSD(i))
			.WillOnce(testing::Return(WRITE_SUCCESS_STRING));
	}

	testShell.fullRead();
}