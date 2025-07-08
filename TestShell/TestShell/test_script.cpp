#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"
#include "shellFixture.cpp"
#include <vector>
#include <random>

using namespace testing;

TEST_F(ShellFixture, FullWriteAndReadCompare) {
	vector<unsigned int> randomValList;
	int unitCount = 5;
	int iterationCount = NUM_OF_LBA / unitCount;

	for (int idx = 0; idx < iterationCount; idx++) {
		unsigned int randomVal = static_cast<unsigned int>(rand());
		randomValList.emplace_back(randomVal);
	}

	for (int idx = 0; idx < iterationCount; idx++) {
		// expect call to write LBA
		for (int unitIdx = 0; unitIdx < unitCount; unitIdx++) {
			int addr = idx * unitCount + unitIdx;
			unsigned int val = randomValList.at(idx);

			EXPECT_CALL(mockSSD, writeToSSD(addr, val))
				.Times(1)
				.WillOnce(Return(WRITE_SUCCESS_STRING));
		}

		// expect call to read LBA
		for (int unitIdx = 0; unitIdx < unitCount; unitIdx++) {
			int addr = idx * unitCount + unitIdx;
			unsigned int val = randomValList.at(idx);
			std::string valString = "0x" + std::to_string(val);

			EXPECT_CALL(mockSSD, readFromSSD(addr))
				.Times(1)
				.WillOnce(Return(valString));
		}
	}

	int actual = testShell.fullWriteAndReadCompare(randomValList);
	EXPECT_EQ(true, actual);
}