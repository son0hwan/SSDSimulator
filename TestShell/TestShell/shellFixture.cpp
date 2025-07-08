#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"

using namespace testing;
class ShellFixture : public Test {
public:
	void SetUp() override {
		std::streambuf* backup = std::cout.rdbuf();
		std::cout.set_rdbuf(localStream.rdbuf());
	}

	void TearDown() override { std::cout.set_rdbuf(backup); }

	std::string getPrintedString() const { return localStream.str(); }

	MockSSD mockSSD;
	TestShell testShell{ &mockSSD };
private:
	std::ostringstream localStream;
	std::streambuf* backup = nullptr;
};