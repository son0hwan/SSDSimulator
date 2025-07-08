#pragma once
#include <string>
#include <iostream>
#include "executor.h"

class TestShellCmdInterface {
public:
	virtual void run() = 0;

	void setExecutor(Executor* executor) {
		this->executor = executor;
	}
protected:
	Executor* executor;
};

class TestShellExitCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellExitCmd() {}

	void run() override {
		
	}
};

class TestShellHelpCmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellHelpCmd() {}

	void run() override {
		std::cout << "\n [HELP]\n";
		std::cout << " -. read [LBA] \n";
		std::cout << " -. write [LBA] [DATA_IN_HEX] \n";
	}
};


class TestShellTestScript1Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellTestScript1Cmd() {}

	void run() override {
	}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellScript2Cmd() {}

	void run() override {
	}
};

class TestShellScript3Cmd : public TestShellCmdInterface {
public:
	// �����ڴ� parser���� �ҷ��� ��
	TestShellScript3Cmd() {}

	void run() override {

	}
};

class TestShellErrorCmd : public TestShellCmdInterface {
 public:
  // �����ڴ� parser���� �ҷ��� ��
  TestShellErrorCmd() {}

  void run() override {
  }
};