#pragma once
#include "shellCmdInterface.h"

constexpr shellCmdInterface* SHELL_EXIT_CMD = nullptr;

class ShellInvalidCmd : public shellCmdInterface {
public:
	ShellInvalidCmd();
	void run() override;
};

class ShellHelpCmd : public shellCmdInterface {
public:
	ShellHelpCmd();
	void run() override;
};

class ShellErrorCmd : public shellCmdInterface {
public:
	ShellErrorCmd();
	void run() override;
};

class ShellReadCmd : public shellCmdInterface {
public:
	ShellReadCmd(long address);
	void run() override;

	long getAddress();
protected:
	const long address;
};


class ShellFullReadCmd : public shellCmdInterface {
public:
	ShellFullReadCmd();
	void run() override;
};

class ShellWriteCmd : public shellCmdInterface {
public:
	ShellWriteCmd(long address, unsigned data);
	void run() override;

	long getAddress();
	long getData();
protected:
	const long address;
	const unsigned data;
};


class ShellFullWriteCmd : public shellCmdInterface {
public:
	ShellFullWriteCmd(unsigned data);
	void run() override;
protected:
	const unsigned data;
};


class ShellScript1Cmd : public shellCmdInterface {
public:
	ShellScript1Cmd();
	void run() override;
};


class ShellScript2Cmd : public shellCmdInterface {
public:
	ShellScript2Cmd();
	void run() override;
};


class ShellScript3Cmd : public shellCmdInterface {
public:
	ShellScript3Cmd();
	void run() override;
private:
	const int MAX_LOOP_COUNT = 200;
};

class ShellEraseCmd : public shellCmdInterface {
public:
	ShellEraseCmd(long address, long size);
	virtual void run() override;

	long getAddress();
	long getSize();
protected:
	const long address;
	const long size;
};

class ShellEraseRangeCmd : public shellCmdInterface {
public:
	ShellEraseRangeCmd(long startAddress, long endAddress);
	void run() override;

	long getStartAddress();
	long getEndAddress();
protected:
	const long startAddress;
	const long endAddress;
};

class ShellScript4Cmd : public shellCmdInterface {
public:
	ShellScript4Cmd();
	void run() override;
};

class ShellFlushCmd : public shellCmdInterface {
public:
	ShellFlushCmd();
	void run() override;
};