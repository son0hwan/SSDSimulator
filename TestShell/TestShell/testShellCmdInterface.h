#pragma once

class TestShellCmdInterface {
 public:
  virtual void run() = 0;
};

constexpr TestShellCmdInterface* TEST_SHELL_EXIT_CMD = nullptr;


class TestShellInvalidCmd : public TestShellCmdInterface {
 public:
  TestShellInvalidCmd() {}

  void run() override {}
};

class TestShellHelpCmd : public TestShellCmdInterface {
 public:
  TestShellHelpCmd() {}

  void run() override {}
};

class TestShellTestScript1Cmd : public TestShellCmdInterface {
 public:
  TestShellTestScript1Cmd() {}

  void run() override {}
};

class TestShellScript2Cmd : public TestShellCmdInterface {
 public:
  TestShellScript2Cmd() {}

  void run() override {}
};

class TestShellScript3Cmd : public TestShellCmdInterface {
 public:
  TestShellScript3Cmd() {}

  void run() override {}
};

class TestShellErrorCmd : public TestShellCmdInterface {
 public:
  TestShellErrorCmd() {}

  void run() override {}
};