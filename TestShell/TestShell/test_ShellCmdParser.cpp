#include <vector>

#include "gmock/gmock.h"
#include "shellCmdParser.h"
#include "stdexcept"
#include "testShellCmdInterface.h"
#include "testShellCmdRead.cpp"
#include "testShellCmdWrite.cpp"
#include "testShellCmdComplex.cpp"

using namespace testing;
using namespace std;

class ShellCmdParserFixture : public Test {
 public:
  template <class T>
  bool isCmdTypeOf(TestShellCmdInterface* command) {
    return (nullptr != dynamic_cast<T*>(command));
  }

  ShellCmdParser cmdParser;
};

TEST_F(ShellCmdParserFixture, ReadWithValidAddress) {
  auto command = cmdParser.getCommand({"read", "0"});

  EXPECT_TRUE(isCmdTypeOf<TestShellReadCmd>(command));
  try {
    TestShellReadCmd* convertedCmd = dynamic_cast<TestShellReadCmd*>(command);
    EXPECT_EQ(0, convertedCmd->getAddress());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, ReadWithInValidCommand) {
  auto command = cmdParser.getCommand({"READ", "3"});
  EXPECT_TRUE(isCmdTypeOf<TestShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, ReadWithInValidAddress) {
  auto command = cmdParser.getCommand({"read", "AAA"});
  EXPECT_TRUE(isCmdTypeOf<TestShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithValidAddress) {
  auto command = cmdParser.getCommand({"write", "3", "0xAAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<TestShellWriteCmd>(command));
  try {
    TestShellWriteCmd* convertedCmd = dynamic_cast<TestShellWriteCmd*>(command);
    EXPECT_EQ(3, convertedCmd->getAddress());
    EXPECT_EQ(0xAAAABBBB, convertedCmd->getData());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress1) {
  auto command = cmdParser.getCommand({"write", "3", "AAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<TestShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress2) {
  auto command = cmdParser.getCommand({"write", "0x333", "AAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<TestShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress3) {
  auto command = cmdParser.getCommand({"write", "3", "0xAAAABBBB", "0xAAABBB"});
  EXPECT_TRUE(isCmdTypeOf<TestShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, ExitCommand) {
  auto command = cmdParser.getCommand({"exit"});
  EXPECT_EQ(TEST_SHELL_EXIT_CMD, command);
}

TEST_F(ShellCmdParserFixture, HelpCommand) {
  auto command = cmdParser.getCommand({"help"});
  EXPECT_TRUE(isCmdTypeOf<TestShellHelpCmd>(command));
}

TEST_F(ShellCmdParserFixture, FullWriteCommand) {
    auto command = cmdParser.getCommand({ "fullwrite", "0xAAAABBBB" });
    EXPECT_TRUE(isCmdTypeOf<TestShellFullWriteCmd>(command));
}

TEST_F(ShellCmdParserFixture, FullReadCommand) {
    auto command = cmdParser.getCommand({ "fullread" });
    EXPECT_TRUE(isCmdTypeOf<TestShellFullReadCmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript1Cmd1) {
    ShellCmdParser cmdParser;
    auto command = cmdParser.getCommand({ "1_FullWriteAndReadCompare" });

    EXPECT_TRUE(isCmdTypeOf<TestShellScript1Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript1Cmd2) {
    ShellCmdParser cmdParser;
    auto command = cmdParser.getCommand({ "1_" });

    EXPECT_TRUE(isCmdTypeOf<TestShellScript1Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript2Cmd1) {
    auto command = cmdParser.getCommand({ "2_PartialLBAWrite" });
    EXPECT_TRUE(isCmdTypeOf<TestShellScript2Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript2Cmd2) {
    auto command = cmdParser.getCommand({ "2_" });
    EXPECT_TRUE(isCmdTypeOf<TestShellScript2Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript3Cmd1) {
    auto command = cmdParser.getCommand({ "3_WriteReadAging" });
    EXPECT_TRUE(isCmdTypeOf<TestShellScript3Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript3Cmd2) {
    auto command = cmdParser.getCommand({ "3_" });
    EXPECT_TRUE(isCmdTypeOf<TestShellScript3Cmd>(command));
}

TEST_F(ShellCmdParserFixture, EraseCmd) {
  auto command = cmdParser.getCommand({"erase"});
  EXPECT_TRUE(isCmdTypeOf<TestShellEraseCmd>(command));
}

TEST_F(ShellCmdParserFixture, EraseRangeCmd) {
  auto command = cmdParser.getCommand({"erase_range"});
  EXPECT_TRUE(isCmdTypeOf<TestShellEraseRangeCmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript4Cmd1) {
  auto command = cmdParser.getCommand({"4_EraseAndWriteAging"});
  EXPECT_TRUE(isCmdTypeOf<TestShellScript4Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript4Cmd2) {
  auto command = cmdParser.getCommand({"4_"});
  EXPECT_TRUE(isCmdTypeOf<TestShellScript4Cmd>(command));
}

TEST_F(ShellCmdParserFixture, FlushCmd) {
  auto command = cmdParser.getCommand({"flush"});
  EXPECT_TRUE(isCmdTypeOf<TestShellScript3Cmd>(command));
}
