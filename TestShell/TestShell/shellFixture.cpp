#pragma once
#include "gmock/gmock.h"
#include "mockExecutor.cpp"
#include "testShell.cpp"

using namespace testing;
class ShellFixture : public Test {
public:
	MockSSD mockSSD;
	TestShell testShell{ &mockSSD };
};