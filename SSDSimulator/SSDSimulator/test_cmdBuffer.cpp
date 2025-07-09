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

	MockCommandBufferStroage mockStorage;
	MockCommandBuffer cmdBuffer{ mockStorage };
};

TEST_F(CommandBufferFixture, addReadCmd) {
	SsdReadCmd* cmd = &SsdReadCmd::getInstance();
	CmdQ_type expected{ cmd };

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, addWriteCmd) {
	SsdWriteCmd* cmd = &SsdWriteCmd::getInstance();
	CmdQ_type expected{};

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, addEraseCmd) {
	SsdEraseCmd* cmd = &SsdEraseCmd::getInstance();
	CmdQ_type expected{};

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd);

	EXPECT_THAT(result, ContainerEq(expected));
}

TEST_F(CommandBufferFixture, add5WriteCmd) {
	///TBD, Need to release writeCmd singleton
}