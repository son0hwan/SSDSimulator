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

bool ShellScript1Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	vector<unsigned int> values;
	int unitCount = 5;
	int iterationCount = NUM_OF_LBA / unitCount;

	for (int idx = 0; idx < iterationCount; idx++) {
		unsigned int randomVal = static_cast<unsigned int>(rand());
		values.emplace_back(randomVal);
	}

	for (int idx = 0; idx < values.size(); idx++) {
		int startIdx = idx * 5;
		for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
			int addr = startIdx + unitIdx;
			unsigned int value = values.at(idx);

			if (executor->writeToSSD(addr, value) == ERROR_STRING) {
				std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
				return false;
			}
		}

		for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
			int addr = startIdx + unitIdx;
			unsigned int value = values.at(idx);
			std::string expectedValString = "PASS_ON_EXE";
			std::string actualValString = executor->readFromSSD(addr);

			if (expectedValString != actualValString) {
				std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
				return false;
			}
		}
	}
	std::cout << "[1_FullWriteAndReadCompare] Done" << std::endl;
	return true;
}

ShellScript2Cmd::ShellScript2Cmd() {
	LOG(std::string(__FUNCTION__) + " has been called");
}

bool ShellScript2Cmd::run() {
	LOG(std::string(__FUNCTION__) + " has been called");

	vector<unsigned int> values;
	for (int i = 0; i < 30; i++) {
		unsigned int randomVal = static_cast<unsigned int>(rand());
		values.emplace_back(randomVal);
	}
	if (values.size() != 30) {
		std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
		return false;
	}

	for (int cnt = 0; cnt < 30; cnt++) {
		executor->writeToSSD(4, values.at(cnt));
		executor->writeToSSD(0, values.at(cnt));
		executor->writeToSSD(3, values.at(cnt));
		executor->writeToSSD(1, values.at(cnt));
		executor->writeToSSD(2, values.at(cnt));

		executor->readFromSSD(0);
		std::string valStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
		for (int addr = 1; addr <= 4; addr++) {
			executor->readFromSSD(addr);
			std::string compStr = getFirstLineFromFile(OUTPUT_FILE_NAME);
			if (valStr != compStr) {
				std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
				return false;
			}

			valStr = compStr;
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

		result = executor->writeToSSD(0, stoul(EXPECTED_STR, nullptr, 16));
		if (result == ERROR_STRING)
			return false;

		executor->readFromSSD(0);
		std::string resStrOf0 = getFirstLineFromFile(OUTPUT_FILE_NAME);

		result = executor->writeToSSD(99, stoul(EXPECTED_STR, nullptr, 16));
		if (result == ERROR_STRING)
			return false;

		executor->readFromSSD(99);
		std::string resStrOf99 = getFirstLineFromFile(OUTPUT_FILE_NAME);

		if (resStrOf0 != resStrOf99) {
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