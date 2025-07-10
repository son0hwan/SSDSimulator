#pragma once
#include "consoleInputStrategy.h"
#include "gmock/gmock.h"

class TestInputStrategy : public ConsoleInputStrategy {
public:
	MOCK_METHOD(bool, hasNextCommand, (), (override));
	MOCK_METHOD(std::string, getNextCommand, (), (override));
};