#pragma once

class TestShellCmdInterface {
public:
	virtual void run() = 0;
};


class TestShellExitCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellExitCmd() {}

	void run() override {}
};

class TestShellHelpCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellHelpCmd() {}

	void run() override {}
};


class TestShellTestScript1Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellTestScript1Cmd() {}

	void run() override {}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellScript2Cmd() {}

	void run() override {}
};

class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellScript3Cmd() {}

	void run() override {}
};