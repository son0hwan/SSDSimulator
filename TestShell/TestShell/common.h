#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "logger.h"

namespace {
	const int SUCCESS = 0;
	const int ERROR = 1;
	const int SSDAccessFail = 2;
	const int NUM_OF_LBA = 100;
	const int MAX_VAL_LEN = 8;
	const int MAX_HEX_LENGTH = 16;
	const int ERASE_UNIT = 10;

	const std::string PASS = "PASS_ON_EXE";
	const std::string FAIL = "FAIL_ON_EXE";
	const std::string OUTPUT_FILE_NAME = "ssd_output.txt";
	const std::string TEST_SAMPLE_DATA = "0x12345678";
	const std::string TEST_SAMPLE_DATA2 = "0x11111111";

	const char* SUCCESS_STRING = "";
	const char* ERROR_STRING = "ERROR";

}

inline std::string genRandomString(int length) {
	std::string result = "0x";
	const char HEX_DIGITS[] = "0123456789ABCDEF";
	for (char c = 0; c < length; ++c) {
		result.append(1, HEX_DIGITS[c]);
	}
	return result;
}

inline std::string getFirstLineFromFile(std::string fileName) {
	std::string line;
	std::ifstream file(fileName);

	if (!file.is_open()) {
		std::cerr << "fail to open file" << std::endl;
		return line;
	}

	std::istream& res = std::getline(file, line);
	if (!res && !res.eof()) {
		std::cout << "fail to read file" << std::endl;
	}

	file.close();

	return line;
}