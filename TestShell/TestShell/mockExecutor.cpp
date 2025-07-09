#pragma once
#include <string>

#include "executor.h"
#include "gmock/gmock.h"

using namespace testing;

class MockSSD : public Executor {
public:
	MOCK_METHOD(std::string, readFromSSD, (int), (override));
	MOCK_METHOD(std::string, writeToSSD, (int, unsigned int), (override));
	MOCK_METHOD(std::string, eraseToSSD, (int, int), (override));
};