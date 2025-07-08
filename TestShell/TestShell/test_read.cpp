#include <random>
#include "mockExecutor.cpp"
#include "testShell.cpp"

#define FULL_READ_COUNT 100
#define MAX_STR_LENGTH 8
#define MAX_ALPHABET_SIZE 26

std::vector<std::string> generateAlphabet() {
	std::vector<std::string> result;
	for (char c = 'A'; c <= 'Z'; ++c) {
		result.emplace_back(1, c);
	}
	return result;
}

TEST(TS_READ, READ_JUST_ONCE) {
	MockSSD mockSSD;
	TestShell testShell{ &mockSSD };
	std::vector<std::string> alphabetList = generateAlphabet();

	std::string EXPECTED_STR = "0x";
	for (int i = 0; i < MAX_STR_LENGTH; i++)
		EXPECTED_STR.append(alphabetList[rand() % MAX_ALPHABET_SIZE]);
		
	EXPECT_CALL(mockSSD, readFromSSD)
		.WillOnce(testing::Return(EXPECTED_STR));

	EXPECT_EQ(EXPECTED_STR, testShell.read(50));
}

TEST(TS_READ, READ_FULL) {
	MockSSD mockSSD;
	TestShell testShell{ &mockSSD };

	for (int i = 0; i < FULL_READ_COUNT; i++) {
		EXPECT_CALL(mockSSD, readFromSSD(i))
			.WillOnce(testing::Return(""));
	}

	testShell.fullRead();
}