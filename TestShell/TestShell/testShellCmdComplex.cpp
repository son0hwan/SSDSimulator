#include "testShellCmdInterface.h"
#include <iostream>
#include <fstream>
#include <vector>

using std::vector;

class TestShellScript1Cmd : public TestShellCmdInterface {
public:
	TestShellScript1Cmd() {}
	void run() override {
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
					return;
				}
			}

			for (int unitIdx = 0; unitIdx < 5; unitIdx++) {
				int addr = startIdx + unitIdx;
				unsigned int value = values.at(idx);
				std::string expectedValString = "PASS_ON_EXE";
				std::string actualValString = executor->readFromSSD(addr);

				if (expectedValString != actualValString) {
					std::cout << "[1_FullWriteAndReadCompare] Fail" << std::endl;
					return;
				}
			}
		}
		std::cout << "[1_FullWriteAndReadCompare] Done" << std::endl;
		return;
	}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
public:
	TestShellScript2Cmd() {}

	std::string readOutputFile() {
		std::string line;
		std::ifstream file("ssd_output.txt");

		if (!file.is_open()) {
			std::cerr << "fail to open file" << std::endl;
			return line;
		}

		if (std::getline(file, line)) {
			; //std::cout << line << std::endl;
		}
		else {
			std::cout << "fail to read file" << std::endl;
		}

		file.close();

		return line;
	}

	void run() override {
		vector<unsigned int> values;
		for (int i = 0; i < 30; i++) {
			unsigned int randomVal = static_cast<unsigned int>(rand());
			values.emplace_back(randomVal);
		}
		if (values.size() != 30) {
			std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
			return;
		}

		for (int cnt = 0; cnt < 30; cnt++) {
			executor->writeToSSD(4, values.at(cnt));
			executor->writeToSSD(0, values.at(cnt));
			executor->writeToSSD(3, values.at(cnt));
			executor->writeToSSD(1, values.at(cnt));
			executor->writeToSSD(2, values.at(cnt));

			executor->readFromSSD(0);
			std::string valStr = readOutputFile();
			for (int addr = 1; addr <= 4; addr++) {
				executor->readFromSSD(addr);
				std::string compStr = readOutputFile();
				if (valStr != compStr) {
					std::cout << "[2_PartialLBAWrite] Fail" << std::endl;
					return;
				}

				valStr = compStr;
			}
		}

		std::cout << "[2_PartialLBAWrite] Done" << std::endl;
		return;
	}

};

class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	TestShellScript3Cmd() {}

	std::vector<std::string> generateAlphabet() {
		std::vector<std::string> result;
		for (char c = 'A'; c <= 'Z'; ++c) {
			result.emplace_back(1, c);
		}
		return result;
	}

	std::string readOutputFile() {
		std::string line;
		std::ifstream file("ssd_output.txt");

		if (!file.is_open()) {
			std::cerr << "fail to open file" << std::endl;
			return line;
		}

		if (std::getline(file, line)) {
			; //std::cout << line << std::endl;
		}
		else {
			std::cout << "fail to read file" << std::endl;
		}

		file.close();

		return line;
	}

	void run() override {
		std::string result;
		std::vector<std::string> alphabetList = generateAlphabet();

		std::string EXPECTED_STR = "0x";
		for (int i = 0; i < MAX_VAL_LEN; i++)
			EXPECTED_STR.append(alphabetList[rand() % MAX_ALPHABET_SIZE]);

		for (int i = 0; i < 200; i++) {
			result = executor->writeToSSD(0, stoi(EXPECTED_STR));
			if (result == ERROR_STRING)
				return;

			executor->readFromSSD(0);
			std::string resStrOf0 = readOutputFile();

			result = executor->writeToSSD(99, stoi(EXPECTED_STR));
			if (result == ERROR_STRING)
				return;

			executor->readFromSSD(99);
			std::string resStrOf99 = readOutputFile();

			if (resStrOf0 != resStrOf99) {
				std::cout << "[3_WriteReadAging] Fail" << std::endl;
				return;
			}
		}

		std::cout << "[3_WriteReadAging] Done" << std::endl;
	}
};