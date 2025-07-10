#pragma once
#include "testShellCmdInterface.h"
#include <vector>

constexpr TestShellCmdInterface* TEST_SHELL_EXIT_CMD = nullptr;

class TestShellInvalidCmd : public TestShellCmdInterface {
public:
	TestShellInvalidCmd();
	bool run() override;
};

class TestShellHelpCmd : public TestShellCmdInterface {
public:
	TestShellHelpCmd();
	bool run() override;
};

class TestShellErrorCmd : public TestShellCmdInterface {
public:
	TestShellErrorCmd();
	bool run() override;
};

class TestShellReadCmd : public TestShellCmdInterface {
public:
	TestShellReadCmd(long address);
	bool run() override;

	long getAddress();
protected:
	const long address;
};


class TestShellFullReadCmd : public TestShellCmdInterface {
public:
	TestShellFullReadCmd();
	bool run() override;
};

class TestShellWriteCmd : public TestShellCmdInterface {
public:
	TestShellWriteCmd(long address, unsigned data);
	bool run() override;

	long getAddress();
	long getData();
protected:
	const long address;
	const unsigned data;
};


class TestShellFullWriteCmd : public TestShellCmdInterface {
public:
	TestShellFullWriteCmd(unsigned data);
	bool run() override;
protected:
	const unsigned data;
};


class TestShellScript1Cmd : public TestShellCmdInterface {
public:
	TestShellScript1Cmd();
<<<<<<< HEAD
	void run() override;
	int writeFiveTimesFromIdx(unsigned int value, int startIdx);
	int readFiveTimesFromIdx(std::vector<unsigned int>& values, int startIdx);
	int checkValueIsSame(unsigned int writeValue, const std::vector<unsigned int>& readValues);
private:
	int unitCount = 5;
	int iterationCount = NUM_OF_LBA / unitCount;
=======
	bool run() override;
>>>>>>> master
};


class TestShellScript2Cmd : public TestShellCmdInterface {
public:
	TestShellScript2Cmd();
	bool run() override;
};


class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	TestShellScript3Cmd();
	bool run() override;
private:
	const int MAX_LOOP_COUNT = 200;
};


class TestShellEraseCmd : public TestShellCmdInterface {
public:
	TestShellEraseCmd(long address, long size);
	virtual bool run() override;

	long getAddress();
	long getSize();
protected:
	const long address;
	const long size;
};

class TestShellEraseRangeCmd : public TestShellEraseCmd {
public:
	TestShellEraseRangeCmd(long startAddress, long endAddress);
	long getStartAddress();
	long getEndAddress();
};

class TestShellScript4Cmd : public TestShellCmdInterface {
public:
	TestShellScript4Cmd();
	bool run() override;
};

class TestShellFlushCmd : public TestShellCmdInterface {
public:
	TestShellFlushCmd();
	bool run() override;
};