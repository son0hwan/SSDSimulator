#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "testShellCmd.h"
#include "common.h"

using std::vector;

TestShellScript1Cmd::TestShellScript1Cmd() {}

void TestShellScript1Cmd::run() {
	vector<unsigned int> values;
	int unitCount = 5;
	int iterationCount = NUM_OF_LBA / unitCount;

	for (int idx = 0; idx < iterationCount; idx++) {
		values.emplace_back(rand());
	}

	for (int idx = 0; idx < values.size(); idx++) {
		int startIdx = idx * 5;
		for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
			int addr = startIdx + unitIdx;
			unsigned int value = values.at(idx);

			if (executor->writeToSSDWithResult(addr, value)) {
				std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
				return;
			}
		}

		for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
			int addr = startIdx + unitIdx;
			unsigned int value;
			if (executor->readFromSSDWithResult(addr, &value)) {
				std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
				return;
			}
		}
	}
	std::cout << "[1_FullWriteAndReadCompare] Done" << std::endl;
	return;
}

TestShellScript2Cmd::TestShellScript2Cmd() {}

void TestShellScript2Cmd::run() {
	vector<unsigned int> values;
	for (int i = 0; i < 30; i++) {
		values.emplace_back(rand());
	}
	if (values.size() != 30) {
		std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
		return;
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
			return;
		}
		for (int addr = 1; addr <= 4; addr++) {
			unsigned int comp;
			if (executor->readFromSSDWithResult(addr, &comp)) {
				std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
				return;
			}
			if (value != comp) {
				std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
				return;
			}
			value = comp;
		}
	}

	std::cout << "[2_PartialLBAWrite] Done" << std::endl;
	return;
}

TestShellScript3Cmd::TestShellScript3Cmd() {}

void TestShellScript3Cmd::run() {
	std::string result;

	for (int i = 0; i < MAX_LOOP_COUNT; i++) {
		std::string EXPECTED_STR = genRandomString(MAX_VAL_LEN);

		if (executor->writeToSSDWithResult(0, stoul(EXPECTED_STR, nullptr, 16)))
			return;
		if (executor->writeToSSDWithResult(99, stoul(EXPECTED_STR, nullptr, 16)))
			return;

		unsigned int resOf0, resOf99;

		executor->readFromSSDWithResult(0, &resOf0);
		executor->readFromSSDWithResult(0, &resOf99);

		if (resOf0 != resOf99) {
			std::cout << "[3_WriteReadAging] Fail" << std::endl;
			return;
		}
	}

	std::cout << "[3_WriteReadAging] Done" << std::endl;
}

TestShellScript4Cmd::TestShellScript4Cmd() {
}

void TestShellScript4Cmd::run() {
}