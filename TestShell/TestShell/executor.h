#pragma once
#include <string>


namespace {
	const int SUCCESS = 0;
	const int ERROR = 1;
	const char* SUCCESS_STRING = "";
	const char* ERROR_STRING = "ERROR";
	const int NUM_OF_LBA = 100;
	const int MAX_VAL_LEN = 8;
	const int MAX_HEX_LENGTH = 16;
	const std::string PASS = "PASS_ON_EXE";
	const std::string FAIL = "FAIL_ON_EXE";
}
	
#define for_each_addr(addr) for (int addr = 0; addr < NUM_OF_LBA; addr++)

class Executor {
public:
	virtual std::string readFromSSD(int address) = 0;
	virtual std::string writeToSSD(int address, int value) = 0;
};