#include "cmdBuffer.h"
#include "gmock/gmock.h"
#include "ssdCmdErase.h"
#include "ssdCmdError.h"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "stdexcept"

using namespace testing;

class MockCommandBufferStroage : public CommandBufferStorage {
public:
	MOCK_METHOD(vector<BufferedCmdInfo*>, getBufferFromStorage, (), (override));
	//MOCK_METHOD(void, setBufferToStorage, (vector<BufferedCmdInfo*>), (override));
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

	mockStorage.setBufferToStorage(v);

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