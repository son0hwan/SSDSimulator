#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "shellCmd.h"
#include "common.h"

using std::vector;

ShellScript1Cmd::ShellScript1Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

int ShellScript1Cmd::writeFiveTimesFromIdx(unsigned int value, int startIdx) {
	for (int i = 0; i < unitCount; i++) {
		int addr = startIdx + unitCount;
		if (executor->writeToSSDWithResult(addr, value))
			return ERROR;
	}
	return SUCCESS;
}

int ShellScript1Cmd::readFiveTimesFromIdx(vector<unsigned int>& values, int startIdx) {
	for (int i = 0; i < unitCount; i++) {
		int addr = startIdx + unitCount;
		unsigned int value;
		if (executor->readFromSSDWithResult(addr, &value))
			return ERROR;

		values.emplace_back(value);
	}
	return SUCCESS;
}

int ShellScript1Cmd::checkValueIsSame(unsigned int writeValue, const vector<unsigned int>& readValues) {
	for (int i = 0; i < unitCount; i++) {
		if (writeValue != readValues.at(i))
			return ERROR;
	}
	return SUCCESS;
}

bool ShellScript1Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	for (int idx = 0; idx < iterationCount; idx++) {
		unsigned int writeValue = rand();
		vector<unsigned int> readValues;

		if (writeFiveTimesFromIdx(writeValue, idx * unitCount)) {
			std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
			return false;
		}
		if (readFiveTimesFromIdx(readValues, idx * unitCount)) {
			std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
			return false;

		}
		if (checkValueIsSame(writeValue, readValues)) {
			std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
			return false;
		}
		std::cout << "[1_FullWriteAndReadCompare] Done" << std::endl;
		return true;
	}
	return true;
}

ShellScript2Cmd::ShellScript2Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellScript2Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	vector<unsigned int> values;
	for (int i = 0; i < 30; i++) {
		values.emplace_back(rand());
	}
	if (values.size() != 30) {
		std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
		return false;
	}

	for (int cnt = 0; cnt < 30; cnt++) {
		executor->writeToSSDWithResult(4, values.at(cnt));
		executor->writeToSSDWithResult(0, values.at(cnt));
		executor->writeToSSDWithResult(3, values.at(cnt));
		executor->writeToSSDWithResult(1, values.at(cnt));
		executor->writeToSSDWithResult(2, values.at(cnt));

		unsigned int value;
		if (executor->readFromSSDWithResult(0, &value)) {
			std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
			return false;
		}
		for (int addr = 1; addr <= 4; addr++) {
			unsigned int comp;
			if (executor->readFromSSDWithResult(addr, &comp)) {
				std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
				return false;
			}
			if (value != comp) {
				std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
				return false;
			}
			value = comp;
		}
	}

	std::cout << "[2_PartialLBAWrite] Done" << std::endl;
	return true; 
}

ShellScript3Cmd::ShellScript3Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellScript3Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	std::string result;

	for (int i = 0; i < MAX_LOOP_COUNT; i++) {
		std::string EXPECTED_STR = genRandomString(MAX_VAL_LEN);

		if (executor->writeToSSDWithResult(0, stoul(EXPECTED_STR, nullptr, 16)))
			return false;
		if (executor->writeToSSDWithResult(99, stoul(EXPECTED_STR, nullptr, 16)))
			return false;

		unsigned int resOf0, resOf99;

		executor->readFromSSDWithResult(0, &resOf0);
		executor->readFromSSDWithResult(0, &resOf99);

		if (resOf0 != resOf99) {
			std::cout << "[3_WriteReadAging] Fail" << std::endl;
			return false;
		}
	}

	std::cout << "[3_WriteReadAging] Done" << std::endl;
	return true;
}

ShellScript4Cmd::ShellScript4Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellScript4Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");
	return true;
}