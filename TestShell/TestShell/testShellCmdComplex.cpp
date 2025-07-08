#include "testShellCmdInterface.h"
#include <iostream>
#include <fstream>
#include <vector>

class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	TestShellScript3Cmd() {}

	std::vector<std::string> generateAlphabet() {
		std::vector<std::string> result;
		const char HEX_DIGITS[] = "0123456789ABCDEF";
		for (char c = 0; c < MAX_HEX_LENGTH; ++c) {
			result.emplace_back(1, HEX_DIGITS[c]);
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

		for (int i = 0; i < 200; i++) {
			std::string EXPECTED_STR = "0x";
			for (int i = 0; i < MAX_VAL_LEN; i++)
				EXPECTED_STR.append(alphabetList[rand() % MAX_HEX_LENGTH]);

			result = executor->writeToSSD(0, stoul(EXPECTED_STR, nullptr, 16));
			if (result == ERROR_STRING)
				return;

			executor->readFromSSD(0);
			std::string resStrOf0 = readOutputFile();

			result = executor->writeToSSD(99, stoul(EXPECTED_STR, nullptr, 16));
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