#include "gmock/gmock.h"
#include "stdexcept"
#include "ssdCmdIncludes.h"
#include <memory>

using namespace testing;

class SsdCmdParserFixture : public Test {
public:
	template <class T>
	bool isCmdTypeOf(const std::shared_ptr<SsdCmdInterface>& command) {
		return (nullptr != std::dynamic_pointer_cast<T>(command));
	}

	SsdCmdParser cmdParser;
	std::vector<std::string> args;
};

TEST_F(SsdCmdParserFixture, EraseWithValidInput) {
	std::vector<std::string> args = { "E", "2", "10" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	auto convertedCmd = std::dynamic_pointer_cast<SsdEraseCmd>(command);
	EXPECT_TRUE(convertedCmd != nullptr); 
	try {
		EXPECT_EQ(2, convertedCmd->getStartAddress());
		EXPECT_EQ(10, convertedCmd->getSize());
	}
	catch (...) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, FlushCommand) {
	std::vector<std::string> args = { "F" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdFlushCmd>(command));
}

TEST_F(SsdCmdParserFixture, ReadWithValidAddress) {
	std::vector<std::string> args = { "R", "3" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	auto convertedCmd = std::dynamic_pointer_cast<SsdReadCmd>(command);
	EXPECT_TRUE(convertedCmd != nullptr);
	try {
		EXPECT_EQ(3, convertedCmd->getAddress());
	}
	catch (std::exception& e) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, WWithValidAddress) {
	std::vector<std::string> args = { "W", "2", "0x00000001" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	auto convertedCmd = std::dynamic_pointer_cast<SsdWriteCmd>(command);
	EXPECT_TRUE(convertedCmd != nullptr);
	try {
		EXPECT_EQ(2, convertedCmd->getAddress());
		EXPECT_EQ(0x00000001, convertedCmd->getData());
	}
	catch (std::exception e) {
		FAIL();
	}
}

TEST_F(SsdCmdParserFixture, InvalidCmd) {
	std::vector<std::string> args = { "BBB" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, ReadWithNonAddress) {
	std::vector<std::string> args = { "R", "BBB" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, WriteWithNonAddress) {
	std::vector<std::string> args = { "W", "BBB", "0x00000001" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, WriteWithNonData) {
	std::vector<std::string> args = { "W", "2", "BBB" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, ReadWithLessArg) {
	std::vector<std::string> args = { "R" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, ReadWithManyArg) {
	std::vector<std::string> args = { "R", "3", "BBB" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, WriteWithLessArg) {
	std::vector<std::string> args = { "W", "2" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, WriteWithManyArg) {
	std::vector<std::string> args = { "W", "2", "0x00000001", "BBB" };
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
}

TEST_F(SsdCmdParserFixture, NonArg) {
	std::vector<std::string> args = {};
	std::shared_ptr<SsdCmdInterface> command = cmdParser.getCommand(args);
	EXPECT_TRUE(isCmdTypeOf<SsdErrorCmd>(command));
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