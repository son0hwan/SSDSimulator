#pragma once
#include "shellCmdInterface.h"
#include <vector>

constexpr shellCmdInterface* SHELL_EXIT_CMD = nullptr;

class ShellInvalidCmd : public shellCmdInterface {
public:
	ShellInvalidCmd();
	bool run() override;
};

class ShellHelpCmd : public shellCmdInterface {
public:
	ShellHelpCmd();
	bool run() override;
};

class ShellErrorCmd : public shellCmdInterface {
public:
	ShellErrorCmd();
	bool run() override;
};

class ShellReadCmd : public shellCmdInterface {
public:
	ShellReadCmd(long address);
	bool run() override;

	long getAddress();
protected:
	const long address;
};


class ShellFullReadCmd : public shellCmdInterface {
public:
	ShellFullReadCmd();
	bool run() override;
};

class ShellWriteCmd : public shellCmdInterface {
public:
	ShellWriteCmd(long address, unsigned data);
	bool run() override;

	long getAddress();
	long getData();
protected:
	const long address;
	const unsigned data;
};


class ShellFullWriteCmd : public shellCmdInterface {
public:
	ShellFullWriteCmd(unsigned data);
	bool run() override;
protected:
	const unsigned data;
};


class ShellScript1Cmd : public shellCmdInterface {
public:
	ShellScript1Cmd();
	bool run() override;
	int writeFiveTimesFromIdx(unsigned int value, int startIdx);
	int readFiveTimesFromIdx(std::vector<unsigned int>&values, int startIdx);
	int checkValueIsSame(unsigned int writeValue, const std::vector<unsigned int>&readValues);
private:
	int unitCount = 5;
	int iterationCount = NUM_OF_LBA / unitCount;
};


class ShellScript2Cmd : public shellCmdInterface {
public:
	ShellScript2Cmd();
	bool run() override;
};


class ShellScript3Cmd : public shellCmdInterface {
public:
	ShellScript3Cmd();
	bool run() override;
private:
	const int MAX_LOOP_COUNT = 200;
};

class ShellEraseCmd : public shellCmdInterface {
public:
	ShellEraseCmd(long address, long size);
	virtual bool run() override;

	long getAddress();
	long getSize();
protected:
	const long address;
	const long size;
};

class ShellEraseRangeCmd : public shellCmdInterface {
public:
	ShellEraseRangeCmd(long startAddress, long endAddress);
	bool run() override;

	long getStartAddress();
	long getEndAddress();
protected:
	const long startAddress;
	const long endAddress;
};

class ShellScript4Cmd : public shellCmdInterface {
public:
	ShellScript4Cmd();
	bool run() override;
private:
	const int MAX_LOOP_COUNT = 30;
};

class ShellFlushCmd : public shellCmdInterface {
public:
	ShellFlushCmd();
	bool run() override;
};