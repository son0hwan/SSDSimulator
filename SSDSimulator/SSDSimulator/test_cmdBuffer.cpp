#include "cmdBuffer.h"
#include "gmock/gmock.h"
#include "ssdCmdIncludes.h"
#include "stdexcept"
#include <memory>

using namespace testing;

class MockCommandBufferStroage : public CommandBufferStorage {
public:
	MOCK_METHOD(vector<BufferedCmdInfo*>, getBufferFromStorage, (), (override));
	MOCK_METHOD(void, setBufferToStorage, (vector<BufferedCmdInfo*>), (override));
};

class MockCommandBuffer : public CommandBuffer {
public:
	MockCommandBuffer(CommandBufferStorage& storage) : CommandBuffer(storage) {}

	void clearBuffer() { bufferingQ.clear(); }
};

class CommandBufferFixture : public Test {
public:
	void SetUp() override {
		cmdBuffer.clearBuffer();
	}

	NiceMock<MockCommandBufferStroage> mockStorage;
	MockCommandBuffer cmdBuffer{ mockStorage };
};

class RealCommandBufferFixture : public Test {
public:
	CommandBuffer& cmdBuffer = CommandBuffer::getInstance();
	void SetUp() override {
		cmdBuffer.ClearBufferingQ();
		//CommandBuffer::getInstance().ClearBufferingQ();
	}
};



TEST_F(CommandBufferFixture, addReadCmd) {
	SsdReadCmd cmd{};
	CmdQ_type expected{ &cmd };

	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, addWriteCmd) {
	SsdWriteCmd cmd{ 0, 0x12345678 };
	CmdQ_type expected{};

	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, addEraseCmd) {
	SsdEraseCmd cmd{ 0, 1 };
	CmdQ_type expected{};

	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, add5WriteCmd) {
	SsdWriteCmd cmd1{ 0, 0x12345678 };
	SsdWriteCmd cmd2{ 1, 0x12345678 };
	SsdWriteCmd cmd3{ 2, 0x12345678 };
	SsdWriteCmd cmd4{ 3, 0x12345678 };
	SsdWriteCmd cmd5{ 4, 0x12345678 };
	SsdWriteCmd cmd6{ 5, 0x12345678 };

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd2);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd4);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd5);
	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd6);

	CmdQ_type expected{ &cmd1, &cmd2, &cmd3, &cmd4, &cmd5 };
	EXPECT_THAT(result, ContainerEq(expected));
}


TEST_F(CommandBufferFixture, flushCmd) {
	SsdWriteCmd cmd1{ 0, 0x12345678 };
	SsdWriteCmd cmd2{ 1, 0x12345678 };
	SsdWriteCmd cmd3{ 2, 0x12345678 };
	SsdFlushCmd cmd4{};

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd2);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd4);

	CmdQ_type expected{ &cmd1, &cmd2, &cmd3 };
	EXPECT_THAT(result, ContainerEq(expected));
}


TEST_F(CommandBufferFixture, cachedReadWithWrite) {
	SsdWriteCmd cmd1{ 0, 0x12345678 };
	SsdReadCmd	cmd2{0};

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	auto result1 = cmdBuffer.addBufferAndGetCmdToRun(&cmd2);

	EXPECT_EQ(result1.size(), 1);

	auto convertedCmd = dynamic_cast<SsdCachedReadCmd*>(result1[0]);
	EXPECT_TRUE(convertedCmd != nullptr);
	try {
		EXPECT_EQ(0, convertedCmd->getAddress());
		EXPECT_EQ(0x12345678, convertedCmd->getReadData());
	}
	catch (std::exception e) {
		FAIL();
	}

	SsdEraseCmd cmd3{ 0, 5 };
	SsdReadCmd	cmd4{ 0 };

	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	auto result2 = cmdBuffer.addBufferAndGetCmdToRun(&cmd4);

	auto convertedCmd2 = dynamic_cast<SsdCachedReadCmd*>(result2[0]);
	EXPECT_TRUE(convertedCmd2 != nullptr);
	try {
		EXPECT_EQ(0, convertedCmd2->getAddress());
		EXPECT_EQ(0, convertedCmd2->getReadData());
	}
	catch (std::exception e) {
		FAIL();
	}

}


TEST_F(CommandBufferFixture, BufferFileUpdateFullCmdQ) {
	SsdWriteCmd cmd1{ 0, 0xBEEFBEEF };
	SsdWriteCmd cmd2{ 1, 0xDEADDEAD };
	SsdWriteCmd cmd3{ 2, 0xABBAABBA };
	SsdWriteCmd cmd4{ 3, 0xAAAABBBB};
	SsdWriteCmd cmd5{ 4, 0x00011112 };

	std::vector<std::string> expected = { "1_W_0_0xBEEFBEEF", "2_W_1_0xDEADDEAD", "3_W_2_0xABBAABBA",  "4_W_3_0xAAAABBBB",  "5_W_4_0x00011112", };

	std::vector<BufferedCmdInfo*> v;
	v.push_back(cmd1.getBufferedCmdInfo());
	v.push_back(cmd2.getBufferedCmdInfo());
	v.push_back(cmd3.getBufferedCmdInfo());
	v.push_back(cmd4.getBufferedCmdInfo());
	v.push_back(cmd5.getBufferedCmdInfo());

	CommandBufferStorage bufferStorage;
	bufferStorage.setBufferToStorage(v);

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	std::vector<std::string> actual = ioManager.getBufferFileList();

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandBufferFixture, BufferFileUpdatePartiallyFullCmdQ) {
	SsdWriteCmd cmd1{ 0, 0xBEEFBEEF };
	std::vector<std::string> expected = { "1_W_0_0xBEEFBEEF", "2_empty", "3_empty",  "4_empty",  "5_empty", };

	std::vector<BufferedCmdInfo*> v;
	v.push_back(cmd1.getBufferedCmdInfo());

	CommandBufferStorage bufferStorage;
	bufferStorage.setBufferToStorage(v);

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	std::vector<std::string> actual = ioManager.getBufferFileList();

	EXPECT_EQ(expected, actual);
}

TEST_F(CommandBufferFixture, BufferFileUpdateEmptyCmdQ) {
	std::vector<std::string> expected = { "1_empty", "2_empty", "3_empty",  "4_empty",  "5_empty", };

	std::vector<BufferedCmdInfo*> v;
	CommandBufferStorage bufferStorage;
	bufferStorage.setBufferToStorage(v);

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	std::vector<std::string> actual = ioManager.getBufferFileList();

	EXPECT_EQ(expected, actual);
}

TEST_F(RealCommandBufferFixture, AllOverlapEraseWithEraseAddress) {
	SsdWriteCmd cmd1{ 1, 0x12345678 };
	SsdWriteCmd cmd2{ 3, 0x12345678 };
	SsdEraseCmd cmd3{ 2, 3 };	// 2-4
	SsdEraseCmd cmd4{ 1, 5 };	// 1-5
	SsdWriteCmd cmd5{ 2, 0x12345678 };
	SsdWriteCmd cmd6{ 4, 0x12345678 };

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd2);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd4);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd5);
	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd6);

	CmdQ_type expected{ &cmd4, &cmd5 };
	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(RealCommandBufferFixture, PartialOverlapEraseWithEraseAddress) {
	SsdWriteCmd cmd1{ 1, 2 };	// 1-2
	SsdEraseCmd cmd2{ 3, 4 };	// 3-6
	SsdEraseCmd cmd3{ 1, 4 };	// 1-5
	SsdEraseCmd cmd4{ 3, 3 };	// 3-5
	SsdWriteCmd cmd5{ 4, 0x12345678 };
	SsdWriteCmd cmd6{ 4, 0x12345678 };

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd2);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd4);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd5);
	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd6);

	CmdQ_type expected{ &cmd2, &cmd3, &cmd4, &cmd5 };
	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(RealCommandBufferFixture, WrtieOverlapErase) {
	SsdWriteCmd cmd1{ 3, 0x12345678 };
	SsdWriteCmd cmd2{ 4, 0x12345678 };
	SsdWriteCmd cmd3{ 3, 0x12345678 };
	SsdWriteCmd cmd4{ 5, 0x12345678 };
	SsdWriteCmd cmd5{ 3, 0x12345678 };
	SsdWriteCmd cmd6{ 6, 0x12345678 };

	cmdBuffer.addBufferAndGetCmdToRun(&cmd1);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd2);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd3);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd4);
	cmdBuffer.addBufferAndGetCmdToRun(&cmd5);
	auto result = cmdBuffer.addBufferAndGetCmdToRun(&cmd6);

	CmdQ_type expected{ &cmd2, &cmd4, &cmd5 };
	EXPECT_THAT(result, ContainerEq(expected));
}
