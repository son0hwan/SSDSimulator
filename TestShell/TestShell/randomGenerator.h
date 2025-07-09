#pragma once
#include <random>
#include "gmock/gmock.h"

class IRandomGenerator {
public:
	virtual unsigned int next() = 0;
};

class RealRandomGenerator : public IRandomGenerator {
public:
	unsigned int next() override {
		return static_cast<unsigned int>(rand());
	}
};

class MockRandomGenerator : public IRandomGenerator {
public:
	MOCK_METHOD(unsigned int, next, (), (override));
};