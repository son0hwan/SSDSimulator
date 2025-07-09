#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdIncludes.h"

using namespace testing;

class SsdCmdParserFixture : public Test {
public:
	template <class T>
	bool isCmdTypeOf(SsdCmdInterface* command) {
		return (nullptr != dynamic_cast<T*>(command));
	}

	SsdCmdParser cmdParser;
	std::vector<std::string> args;
};

TEST_F(SsdCmdParserFixture, EraseWithValidInput) {
	std::vector<std::string> args = { "E", "2", "10" };
	SsdCmdInterface* command = cmdParser.getCommand(args);

	EXPECT_TRUE(nullptr != dynamic_cast<SsdEraseCmd*>(command));
	try {
		SsdEraseCmd* convertedCmd = dynamic_cast<SsdEraseCmd*>(command);
		EXPECT_EQ(2, convertedCmd->getStartAddress());
		EXPECT_EQ(10, convertedCmd->getSize());
	}
	catch (std::exception e) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, FlushCommand) {
	std::vector<std::string> args = { "F" };
	SsdCmdInterface* command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdFlushCmd>(command));
}

TEST_F(SsdCmdParserFixture, ReadWithValidAddress) {
	std::vector<std::string> args = { "R", "3" };
	SsdCmdInterface* command = cmdParser.getCommand(args);

	EXPECT_TRUE(nullptr != dynamic_cast<SsdReadCmd*>(command));
	try {
		SsdReadCmd* convertedCmd = dynamic_cast<SsdReadCmd*>(command);
		EXPECT_EQ(3, convertedCmd->getAddress());
	}
	catch (std::exception& e) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, WWithValidAddress) {
	std::vector<std::string> args = { "W", "2", "0x00000001" };
	SsdCmdInterface* command = cmdParser.getCommand(args);

	EXPECT_TRUE(nullptr != dynamic_cast<SsdWriteCmd*>(command));
	try {
		SsdWriteCmd* convertedCmd = dynamic_cast<SsdWriteCmd*>(command);
		EXPECT_EQ(2, convertedCmd->getAddress());
		EXPECT_EQ(0x00000001, convertedCmd->getData());
	}
	catch (std::exception e) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, InvalidCmd) {
	std::vector<std::string> args = { "BBB" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, ReadWithNonAddress) {
	std::vector<std::string> args = { "R", "BBB" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, WriteWithNonAddress) {
	std::vector<std::string> args = { "W", "BBB", "0x00000001" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, WriteWithNonData) {
	std::vector<std::string> args = { "W", "2", "BBB" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, ReadWithLessArg) {
	std::vector<std::string> args = { "R" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, ReadWithManyArg) {
	std::vector<std::string> args = { "R", "3", "BBB" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, WriteWithLessArg) {
	std::vector<std::string> args = { "W", "2" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, WriteWithManyArg) {
	std::vector<std::string> args = { "W", "2", "0x00000001", "BBB" };
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}

TEST_F(SsdCmdParserFixture, NonArg) {
	std::vector<std::string> args = {};
	SsdCmdInterface* result = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(result));
}


TEST_F(SsdCmdParserFixture, EncodeWriteCmd) {
	std::vector<std::string> result;
	std::vector<std::string> expected = { "W", "3", "0x12345678" };
	SsdWriteCmd cmd{ 3, 0x12345678 };

	result = cmdParser.getEncodedString(&cmd);
	EXPECT_EQ(result, expected);
}


TEST_F(SsdCmdParserFixture, EncodeEraseCmd) {
	std::vector<std::string> result;
	std::vector<std::string> expected = { "E", "0", "10" };
	SsdEraseCmd cmd{ 0, 10 };

	result = cmdParser.getEncodedString(&cmd);
	EXPECT_EQ(result, expected);
}

TEST_F(SsdCmdParserFixture, EncodeCmd) {
	std::vector<std::string> result;
	std::vector<std::string> expected = {};
	SsdReadCmd cmd{ 0 };

	result = cmdParser.getEncodedString(&cmd);
	EXPECT_EQ(result, expected);
}