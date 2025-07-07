#pragma once
#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellWriteCmd(long address, long data) : address(address), data(data) {}

	void run() override {}

private:
	const long address;
	const long data;
};


class TestShellFullWriteCmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellFullWriteCmd() {}

	void run() override {}
};