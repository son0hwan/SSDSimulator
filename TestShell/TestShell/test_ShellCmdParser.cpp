#include <vector>

#include "gmock/gmock.h"
#include "shellCmdParser.h"
#include "stdexcept"
#include "shellCmd.h"

using namespace testing;
using namespace std;

class ShellCmdParserFixture : public Test {
 public:
  template <class T>
  bool isCmdTypeOf(shellCmdInterface* command) {
    return (nullptr != dynamic_cast<T*>(command));
  }

  ShellCmdParser cmdParser;
};

TEST_F(ShellCmdParserFixture, ReadWithValidAddress) {
  auto command = cmdParser.getCommand({"read", "0"});

  EXPECT_TRUE(isCmdTypeOf<ShellReadCmd>(command));
  try {
    ShellReadCmd* convertedCmd = dynamic_cast<ShellReadCmd*>(command);
    EXPECT_EQ(0, convertedCmd->getAddress());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, ReadWithInValidCommand) {
  auto command = cmdParser.getCommand({"READ", "3"});
  EXPECT_TRUE(isCmdTypeOf<ShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, ReadWithInValidAddress) {
  auto command = cmdParser.getCommand({"read", "AAA"});
  EXPECT_TRUE(isCmdTypeOf<ShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithValidAddress) {
  auto command = cmdParser.getCommand({"write", "3", "0xAAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<ShellWriteCmd>(command));
  try {
    ShellWriteCmd* convertedCmd = dynamic_cast<ShellWriteCmd*>(command);
    EXPECT_EQ(3, convertedCmd->getAddress());
    EXPECT_EQ(0xAAAABBBB, convertedCmd->getData());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress1) {
  auto command = cmdParser.getCommand({"write", "3", "AAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<ShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress2) {
  auto command = cmdParser.getCommand({"write", "0x333", "AAAABBBB"});
  EXPECT_TRUE(isCmdTypeOf<ShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, WriteWithInValidAddress3) {
  auto command = cmdParser.getCommand({"write", "3", "0xAAAABBBB", "0xAAABBB"});
  EXPECT_TRUE(isCmdTypeOf<ShellErrorCmd>(command));
}

TEST_F(ShellCmdParserFixture, ExitCommand) {
  auto command = cmdParser.getCommand({"exit"});
  EXPECT_EQ(SHELL_EXIT_CMD, command);
}

TEST_F(ShellCmdParserFixture, HelpCommand) {
  auto command = cmdParser.getCommand({"help"});
  EXPECT_TRUE(isCmdTypeOf<ShellHelpCmd>(command));
}

TEST_F(ShellCmdParserFixture, FullWriteCommand) {
    auto command = cmdParser.getCommand({ "fullwrite", "0xAAAABBBB" });
    EXPECT_TRUE(isCmdTypeOf<ShellFullWriteCmd>(command));
}

TEST_F(ShellCmdParserFixture, FullReadCommand) {
    auto command = cmdParser.getCommand({ "fullread" });
    EXPECT_TRUE(isCmdTypeOf<ShellFullReadCmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript1Cmd1) {
    ShellCmdParser cmdParser;
    auto command = cmdParser.getCommand({ "1_FullWriteAndReadCompare" });

    EXPECT_TRUE(isCmdTypeOf<ShellScript1Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript1Cmd2) {
    ShellCmdParser cmdParser;
    auto command = cmdParser.getCommand({ "1_" });

    EXPECT_TRUE(isCmdTypeOf<ShellScript1Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript2Cmd1) {
    auto command = cmdParser.getCommand({ "2_PartialLBAWrite" });
    EXPECT_TRUE(isCmdTypeOf<ShellScript2Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript2Cmd2) {
    auto command = cmdParser.getCommand({ "2_" });
    EXPECT_TRUE(isCmdTypeOf<ShellScript2Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript3Cmd1) {
    auto command = cmdParser.getCommand({ "3_WriteReadAging" });
    EXPECT_TRUE(isCmdTypeOf<ShellScript3Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript3Cmd2) {
    auto command = cmdParser.getCommand({ "3_" });
    EXPECT_TRUE(isCmdTypeOf<ShellScript3Cmd>(command));
}

TEST_F(ShellCmdParserFixture, EraseCmd) {
  auto command = cmdParser.getCommand({"erase", "0", "20"});
  EXPECT_TRUE(isCmdTypeOf<ShellEraseCmd>(command));
  try {
    auto* convertedCmd = dynamic_cast<ShellEraseCmd*>(command);
    EXPECT_EQ(0, convertedCmd->getAddress());
    EXPECT_EQ(20, convertedCmd->getSize());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, EraseRangeCmd) {
  auto command = cmdParser.getCommand({"erase_range", "20", "22"});
  EXPECT_TRUE(isCmdTypeOf<ShellEraseRangeCmd>(command));
  try {
    auto* convertedCmd = dynamic_cast<ShellEraseRangeCmd*>(command);
    EXPECT_EQ(20, convertedCmd->getStartAddress());
    EXPECT_EQ(22, convertedCmd->getEndAddress());
  } catch (...) {
    FAIL();
  }
}

TEST_F(ShellCmdParserFixture, ShellTestScript4Cmd1) {
  auto command = cmdParser.getCommand({"4_EraseAndWriteAging"});
  EXPECT_TRUE(isCmdTypeOf<ShellScript4Cmd>(command));
}

TEST_F(ShellCmdParserFixture, ShellTestScript4Cmd2) {
  auto command = cmdParser.getCommand({"4_"});
  EXPECT_TRUE(isCmdTypeOf<ShellScript4Cmd>(command));
}

TEST_F(ShellCmdParserFixture, FlushCmd) {
  auto command = cmdParser.getCommand({"flush"});
  EXPECT_TRUE(isCmdTypeOf<ShellFlushCmd>(command));
}