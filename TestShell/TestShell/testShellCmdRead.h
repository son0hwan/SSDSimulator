#pragma once
#include "testShellCmdInterface.h"

class TestShellReadCmd : public TestShellCmdInterface {
 public:
  // �����ڴ� parser���� �ҷ��� ��
  TestShellReadCmd(long address) : address(address) {}

  void run() override {}

  long getAddress() { return address; }

 private:
  const long address;
};

class TestShellFullReadCmd : public TestShellCmdInterface {
 public:
  // �����ڴ� parser���� �ҷ��� ��
  TestShellFullReadCmd() {}

  void run() override {}
};