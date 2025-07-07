#include <random>
#include "mockExecutor.cpp"
#include "testShell.cpp"

TEST(TS_READ, READ_JUST_ONCE) {
	MockSSD mockSSD;
	TestShell testShell{ &mockSSD };

	const std::string EXPECTED_STR = "0xABCDEFGH";

	EXPECT_CALL(mockSSD, readFromSSD)
		.WillOnce(testing::Return(EXPECTED_STR));

	EXPECT_EQ(EXPECTED_STR, testShell.read(50));
}

TEST(TS_READ, READ_FULL) {
	MockSSD mockSSD;
	const std::string EXPECTED_STR = "0xABCDEFGH";

	for (int i = 0; i < 100; i++) {
		srand(i);
		EXPECT_CALL(mockSSD, readFromSSD)
			.WillOnce(testing::Return(EXPECTED_STR));
		EXPECT_EQ(EXPECTED_STR, mockSSD.readFromSSD(rand() % 100));
	}
}