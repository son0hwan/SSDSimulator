#pragma once
#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellWriteCmd(long address, long data) : address(address), data(data) {}

	void run() override {}

private:
	const long address;
	const long data;
};


class TestShellFullWriteCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellFullWriteCmd() {}

	void run() override {}
};