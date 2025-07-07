#include "gmock/gmock.h"
#include "executor.h"
#include <string>

using namespace testing;

class MockSSD :public Executor {
public:
	MOCK_METHOD(std::string, readFromSSD, (int), (override));
	MOCK_METHOD(std::string, writeToSSD, (int, int), (override));
};