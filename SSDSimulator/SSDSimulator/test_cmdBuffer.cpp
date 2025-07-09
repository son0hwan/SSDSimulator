#include "cmdBuffer.h"
#include "gmock/gmock.h"
#include "ssdCmdErase.h"
#include "ssdCmdError.h"
#include "ssdCmdRead.h"
#include "ssdCmdWrite.h"
#include "stdexcept"

using namespace testing;

class MockCommandBufferStroage : public CommandBufferStroage {
public:
	MOCK_METHOD(CmdQ_type, getBufferFromStorage, (), (override));
	MOCK_METHOD(void, setBufferToStorage, (CmdQ_type), (override));
};

class MockCommandBuffer : public CommandBuffer {
public:
	MockCommandBuffer(CommandBufferStroage& storage) : CommandBuffer(storage) {}

	void clearBuffer() { cmdQ.clear(); }
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