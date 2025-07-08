#pragma once
#include "testShellCmdInterface.h"

class TestShellWriteCmd : public TestShellCmdInterface {
 public:
  // �����ڴ� parser���� �ҷ��� ��
  TestShellWriteCmd(long address, unsigned data)
      : address(address), data(data) {}

  void run() override {}

  long getAddress() { return address; }
  long getData() { return data; }

 private:
  const long address;
  const unsigned data;
};

class TestShellFullWriteCmd : public TestShellCmdInterface {
 public:
  // �����ڴ� parser���� �ҷ��� ��
  TestShellFullWriteCmd() {}

  void run() override {}
};