#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "shell.cpp"
#include "testInputStrategy.h"

using namespace testing;
class ShellFixture : public Test {
public:
	void SetUp() override {
		std::streambuf* backup = std::cout.rdbuf();
		std::cout.set_rdbuf(localStream.rdbuf());
		mockSSD.setRandomGenerator(&mockRandomGenerator);
	}

	void TearDown() override { std::cout.set_rdbuf(backup); }

	std::string getPrintedString() const { return localStream.str(); }
	
	unsigned int rand() {
		return mockSSD.rand();
	}

	void command(std::string cmd) {
		EXPECT_CALL(testInputStrategy, hasNextCommand)
			.WillOnce(Return(true))
			.WillOnce(Return(false));
		EXPECT_CALL(testInputStrategy, getNextCommand)
			.WillOnce(Return(cmd));

		testShell.run();
	}

	MockSSD mockSSD;
	TestInputStrategy testInputStrategy;
	TestShell testShell{ &mockSSD, &testInputStrategy };
	MockRandomGenerator mockRandomGenerator;

	const std::string testDataStr = "0x12345678";
	const unsigned int testData = 0x12345678;
private:
	std::ostringstream localStream;
	std::streambuf* backup = nullptr;
};