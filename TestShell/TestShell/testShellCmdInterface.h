#pragma once

class TestShellCmdInterface {
public:
	virtual void run() = 0;
};


class TestShellExitCmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellExitCmd() {}

	void run() override {}
};

class TestShellHelpCmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellHelpCmd() {}

	void run() override {}
};


class TestShellTestScript1Cmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellTestScript1Cmd() {}

	void run() override {}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellScript2Cmd() {}

	void run() override {}
};

class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	// 생성자는 parser에서 불러줄 것
	TestShellScript3Cmd() {}

	void run() override {}
};