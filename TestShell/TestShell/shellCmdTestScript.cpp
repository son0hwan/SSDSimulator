#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "shellCmd.h"
#include "common.h"

using std::vector;

ShellScript1Cmd::ShellScript1Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "1_FullWriteAndReadCompare";
}

int ShellScript1Cmd::writeFiveTimesFromIdx(unsigned int value, int startIdx) {
	for (int unitIdx = 0; unitIdx < unitCount; unitIdx++) {
		int addr = startIdx + unitIdx;
		if (executor->writeToSSDWithResult(addr, value))
			return ERROR;
	}
	return SUCCESS;
}

int ShellScript1Cmd::readFiveTimesFromIdx(vector<unsigned int>& values, int startIdx) {
	for (int unitIdx = 0; unitIdx < unitCount; unitIdx++) {
		int addr = startIdx + unitIdx;
		unsigned int value;
		if (executor->readFromSSDWithResult(addr, &value))
			return ERROR;

		values.emplace_back(value);
	}
	return SUCCESS;
}

int ShellScript1Cmd::checkValueIsSame(unsigned int writeValue, const vector<unsigned int>& readValues) {
	for (int unitIdx = 0; unitIdx < unitCount; unitIdx++) {
		if (writeValue != readValues.at(unitIdx))
			return ERROR;
	}
	return SUCCESS;
}

bool ShellScript1Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	for (int idx = 0; idx < iterationCount; idx++) {
		unsigned int writeValue = rand();
		vector<unsigned int> readValues;

		if (result = writeFiveTimesFromIdx(writeValue, idx * unitCount))
			break;
		if (result = readFiveTimesFromIdx(readValues, idx * unitCount))
			break;
		if (result = checkValueIsSame(writeValue, readValues))
			break;
	}
	printResult();
	return isCmdSuccess();
}

ShellScript2Cmd::ShellScript2Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "2_PartialLBAWrite";
}

bool ShellScript2Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	vector<unsigned int> values;
	for (int i = 0; i < 30; i++) {
		values.emplace_back(rand());
	}
	if (values.size() != 30) {
		result = ERROR;
		goto ret;
	}

	for (int cnt = 0; cnt < 30; cnt++) {
		executor->writeToSSDWithResult(4, values.at(cnt));
		executor->writeToSSDWithResult(0, values.at(cnt));
		executor->writeToSSDWithResult(3, values.at(cnt));
		executor->writeToSSDWithResult(1, values.at(cnt));
		executor->writeToSSDWithResult(2, values.at(cnt));

		unsigned int value;
		if (result = executor->readFromSSDWithResult(0, &value)) {
			goto ret;
		}
		for (int addr = 1; addr <= 4; addr++) {
			unsigned int comp;
			if (result = executor->readFromSSDWithResult(addr, &comp)) {
				goto ret;
			}
			if (value != comp) {
				result = ERROR;
				goto ret;
			}
			value = comp;
		}
	}
ret:
	printResult();
	return isCmdSuccess(); 
}

ShellScript3Cmd::ShellScript3Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "3_WriteReadAging";
}

bool ShellScript3Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	for (int i = 0; i < MAX_LOOP_COUNT; i++) {
		std::string EXPECTED_STR = genRandomString(MAX_VAL_LEN);

		if (result = executor->writeToSSDWithResult(0, stoul(EXPECTED_STR, nullptr, 16)))
			goto ret;
		if (result = executor->writeToSSDWithResult(99, stoul(EXPECTED_STR, nullptr, 16)))
			goto ret;

		unsigned int resOf0, resOf99;

		executor->readFromSSDWithResult(0, &resOf0);
		executor->readFromSSDWithResult(0, &resOf99);

		if (resOf0 != resOf99) {
			result = ERROR;
			goto ret;
		}
	}
ret:
	printResult();
	return isCmdSuccess();
}

ShellScript4Cmd::ShellScript4Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
	cmdName = "4_EraseAndWriteAging";
}

bool ShellScript4Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");


	if (result = executor->eraseToSSDWithResult(0, 3))
		goto ret;

	for (int i = 0; i < MAX_LOOP_COUNT; i++) {
		for (int address = 2; address <= 98; address += 2) {
			std::string FIRST_RANDOM_STR = genRandomString(MAX_VAL_LEN);
			std::string SECOND_RANDOM_STR = genRandomString(MAX_VAL_LEN);

			if (result = executor->writeToSSDWithResult(address, stoul(FIRST_RANDOM_STR, nullptr, 16)))
				goto ret;

			if (result = executor->writeToSSDWithResult(address, stoul(SECOND_RANDOM_STR, nullptr, 16)))
				goto ret;

			int erase_unit = ((address == 98) ? 2 : 3);
			if (result = executor->eraseToSSDWithResult(address, erase_unit))
				goto ret;
		}
	}
ret:
	printResult();
	return isCmdSuccess();
}