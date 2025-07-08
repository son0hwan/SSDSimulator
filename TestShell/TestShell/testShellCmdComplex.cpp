#include "testShellCmdInterface.h"
#include <iostream>
#include <fstream>
#include <vector>

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