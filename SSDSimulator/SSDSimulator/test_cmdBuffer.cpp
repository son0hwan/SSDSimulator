#include "bufferedCmdInfo.h"
#include "cmdBuffer.h"
#include "gmock/gmock.h"
#include "ssdCmdIncludes.h"
#include "stdexcept"
using namespace testing;

class MockCommandBufferStroage : public CommandBufferStorage {
public:
	MOCK_METHOD(vector<BufferedCmdInfo*>, getBufferFromStorage, (), (override));
	MOCK_METHOD(void, setBufferToStorage, (vector<BufferedCmdInfo*>), (override));
};

class TestCommandBuffer : public CommandBuffer {
public:
	TestCommandBuffer(CommandBufferStorage& storage) : CommandBuffer(storage) {}

	void clearBuffer() { bufferingQ.clear(); }

	CmdQ_type getAllBufferedCmd() {
		std::vector<SsdCmdInterface*> outstandingQ;
		for (auto bufferedInfo : bufferingQ) {
			outstandingQ.push_back(bufferedInfo->getCmd());
		}
		return outstandingQ;
	}

};

class CommandBufferFixture : public Test {
public:
	void SetUp() override {
		cmdBuffer.clearBuffer();
	}

	bool isCachedReadCmd(SsdCmdInterface* cmd, uint32_t expectedAddress, uint32_t expectedData) {
		auto convertedCmd = dynamic_cast<SsdCachedReadCmd*>(cmd);
		if (nullptr == convertedCmd) return false;
		try {
			EXPECT_EQ(expectedAddress, convertedCmd->getAddress());
			EXPECT_EQ(expectedData, convertedCmd->getReadData());
			return true;
		}
		catch (...) {
			return false;
		}
	}

	void CHECK_OUTSTANING_Q(CmdQ_type actual, CmdQ_type expected) {
		EXPECT_THAT(actual, ContainerEq(expected));
	}

	void CHECK_OUTSTANING_Q_WRITE(CmdQ_type actual, CmdQ_type expected) {
		EXPECT_TRUE(isLastCmdCachedWrite(actual));
		EXPECT_THAT(getWithoutLastCmd(actual), ContainerEq(expected));
	}

	bool isLastCmdCachedWrite(CmdQ_type resultVector) {
		return nullptr != dynamic_cast<SsdCachedWriteCmd*>(resultVector[resultVector.size() - 1]);
	}

	CmdQ_type getWithoutLastCmd(CmdQ_type resultVector) {
		return { resultVector.begin(), resultVector.end() - 1 };
	}

	NiceMock<MockCommandBufferStroage> mockStorage;
	TestCommandBuffer cmdBuffer{ mockStorage };
};

TEST_F(CommandBufferFixture, addReadCmd) {
	auto cmd = std::make_shared<SsdReadCmd>();

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd.get());
	CHECK_OUTSTANING_Q(result, CmdQ_type{ cmd.get() });
}

TEST_F(CommandBufferFixture, addWriteCmd) {
	auto cmd = std::make_shared<SsdWriteCmd>(0, 0x12345678);

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd.get());
	CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{});
}

TEST_F(CommandBufferFixture, addEraseCmd) {
	auto cmd = std::make_shared<SsdEraseCmd>(0, 1);

	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd.get());
	CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{});
}

TEST_F(CommandBufferFixture, add5WriteCmd) {
	auto cmd1 = std::make_shared<SsdWriteCmd>(0, 0x12345678);
	auto cmd2 = std::make_shared<SsdWriteCmd>(1, 0x12345678);
	auto cmd3 = std::make_shared<SsdWriteCmd>(2, 0x12345678);
	auto cmd4 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
	auto cmd5 = std::make_shared<SsdWriteCmd>(4, 0x12345678);
	auto cmd6 = std::make_shared<SsdWriteCmd>(5, 0x12345678);

	cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd5.get());
	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd6.get());

	CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{ cmd1.get(), cmd2.get(), cmd3.get(), cmd4.get(), cmd5.get() });
	EXPECT_THAT(cmdBuffer.getAllBufferedCmd(), ContainerEq(CmdQ_type{ cmd6.get() }));
}

TEST_F(CommandBufferFixture, flushCmd) {
	auto cmd1 = std::make_shared<SsdWriteCmd>(0, 0x12345678);
	auto cmd2 = std::make_shared<SsdWriteCmd>(1, 0x12345678);
	auto cmd3 = std::make_shared<SsdWriteCmd>(2, 0x12345678);
	auto cmd4 = std::make_shared<SsdFlushCmd>();

	cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
	cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
	auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());

	CHECK_OUTSTANING_Q(result, CmdQ_type{ cmd1.get(), cmd2.get(), cmd3.get() });
	EXPECT_THAT(cmdBuffer.getAllBufferedCmd(), ContainerEq(CmdQ_type{}));
}

TEST_F(CommandBufferFixture, cachedReadWithWrite) {
	auto cmd1 = std::make_shared<SsdWriteCmd>(0, 0x12345678);
	auto cmd2 = std::make_shared<SsdReadCmd>(0);

	cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
	auto result1 = cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());

	EXPECT_EQ(result1.size(), 1);
	EXPECT_TRUE(isCachedReadCmd(result1[0], 0, 0x12345678));

	auto cmd3 = std::make_shared<SsdEraseCmd>(0, 5);
	auto cmd4 = std::make_shared<SsdReadCmd>(0);
	cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
	auto result2 = cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());

	EXPECT_EQ(result2.size(), 1);
	EXPECT_TRUE(isCachedReadCmd(result2[0], 0, 0));
}


TEST_F(CommandBufferFixture, BufferFileUpdateFullCmdQ) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(0, 0xBEEFBEEF);
    auto cmd2 = std::make_shared<SsdWriteCmd>(1, 0xDEADDEAD);
    auto cmd3 = std::make_shared<SsdWriteCmd>(2, 0xABBAABBA);
    auto cmd4 = std::make_shared<SsdWriteCmd>(3, 0xAAAABBBB);
    auto cmd5 = std::make_shared<SsdWriteCmd>(4, 0x00011112);

    std::vector<std::string> expected = {
        "1_W_0_0xBEEFBEEF", "2_W_1_0xDEADDEAD", "3_W_2_0xABBAABBA",
        "4_W_3_0xAAAABBBB", "5_W_4_0x00011112"
    };

    std::vector<BufferedCmdInfo*> v{
        cmd1->getBufferedCmdInfo(),
        cmd2->getBufferedCmdInfo(),
        cmd3->getBufferedCmdInfo(),
        cmd4->getBufferedCmdInfo(),
        cmd5->getBufferedCmdInfo()
    };

    CommandBufferStorage bufferStorage;
    bufferStorage.setBufferToStorage(v);

    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    std::vector<std::string> actual = ioManager.buffer().getBufferFileList();

    EXPECT_EQ(expected, actual);
}

TEST_F(CommandBufferFixture, BufferFileUpdatePartiallyFullCmdQ) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(0, 0xBEEFBEEF);
    std::vector<std::string> expected = {
        "1_W_0_0xBEEFBEEF", "2_empty", "3_empty", "4_empty", "5_empty"
    };

    std::vector<BufferedCmdInfo*> v{
        cmd1->getBufferedCmdInfo()
    };

    CommandBufferStorage bufferStorage;
    bufferStorage.setBufferToStorage(v);

    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    std::vector<std::string> actual = ioManager.buffer().getBufferFileList();

    EXPECT_EQ(expected, actual);
}

TEST_F(CommandBufferFixture, BufferFileUpdateEmptyCmdQ) {
    std::vector<std::string> expected = {
        "1_empty", "2_empty", "3_empty", "4_empty", "5_empty"
    };

    std::vector<BufferedCmdInfo*> v;
    CommandBufferStorage bufferStorage;
    bufferStorage.setBufferToStorage(v);

    IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
    std::vector<std::string> actual = ioManager.buffer().getBufferFileList();

    EXPECT_EQ(expected, actual);
}

TEST_F(CommandBufferFixture, AllOverlapEraseWithEraseAddress) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(1, 0x12345678);
    auto cmd2 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
    auto cmd3 = std::make_shared<SsdEraseCmd>(2, 3);  // 2-4
    auto cmd4 = std::make_shared<SsdEraseCmd>(1, 5);  // 1-5
    auto cmd5 = std::make_shared<SsdWriteCmd>(2, 0x12345678);
    auto cmd6 = std::make_shared<SsdWriteCmd>(4, 0x12345678);

    cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd5.get());
    auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd6.get());

    CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{});
    CmdQ_type actualBufferingQ = cmdBuffer.popAllBufferToOutstandingQ();
    CmdQ_type expectedBufferingQ{ cmd4.get(), cmd5.get(), cmd6.get() };

    EXPECT_EQ(expectedBufferingQ, actualBufferingQ);
}

TEST_F(CommandBufferFixture, PartialOverlapEraseWithEraseAddress) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(1, 2);       // 1-2
    auto cmd2 = std::make_shared<SsdEraseCmd>(3, 4);       // 3-6
    auto cmd3 = std::make_shared<SsdEraseCmd>(1, 4);       // 1-5
    auto cmd4 = std::make_shared<SsdEraseCmd>(3, 3);       // 3-5
    auto cmd5 = std::make_shared<SsdWriteCmd>(4, 0x12345678);
    auto cmd6 = std::make_shared<SsdWriteCmd>(4, 0x12345678);

    cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd5.get());
    auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd6.get());

    CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{});
    CmdQ_type actualBufferingQ = cmdBuffer.popAllBufferToOutstandingQ();
    CmdQ_type expectedBufferingQ{ cmd2.get(), cmd3.get(), cmd4.get(), cmd6.get() };

    EXPECT_EQ(expectedBufferingQ, actualBufferingQ);
}

TEST_F(CommandBufferFixture, WriteOverlapErase) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
    auto cmd2 = std::make_shared<SsdWriteCmd>(4, 0x12345678);
    auto cmd3 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
    auto cmd4 = std::make_shared<SsdWriteCmd>(5, 0x12345678);
    auto cmd5 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
    auto cmd6 = std::make_shared<SsdWriteCmd>(6, 0x12345678);

    cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd5.get());
    auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd6.get());

    CHECK_OUTSTANING_Q_WRITE(result, CmdQ_type{});
    CmdQ_type actualBufferingQ = cmdBuffer.popAllBufferToOutstandingQ();
    CmdQ_type expectedBufferingQ{ cmd2.get(), cmd4.get(), cmd5.get(), cmd6.get() };

    EXPECT_EQ(expectedBufferingQ, actualBufferingQ);
}

TEST_F(CommandBufferFixture, WriteLaterOverlapErase) {
    auto cmd1 = std::make_shared<SsdWriteCmd>(1, 0x12345678);
    auto cmd2 = std::make_shared<SsdWriteCmd>(2, 0x12345678);
    auto cmd3 = std::make_shared<SsdWriteCmd>(3, 0x12345678);
    auto cmd4 = std::make_shared<SsdWriteCmd>(4, 0x12345678);
    auto cmd5 = std::make_shared<SsdWriteCmd>(5, 0x12345678);
    auto cmd6 = std::make_shared<SsdWriteCmd>(5, 0x12345678);

    cmdBuffer.addBufferAndGetCmdToRun(cmd1.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd2.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd3.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd4.get());
    cmdBuffer.addBufferAndGetCmdToRun(cmd5.get());
    auto result = cmdBuffer.addBufferAndGetCmdToRun(cmd6.get());

    CmdQ_type expectedOutstandingQ{ cmd1.get(), cmd2.get(), cmd3.get(), cmd4.get(), cmd5.get() };
    CHECK_OUTSTANING_Q_WRITE(result, expectedOutstandingQ);

    CmdQ_type actualBufferingQ = cmdBuffer.popAllBufferToOutstandingQ();
    CmdQ_type expectedBufferingQ{ cmd6.get() };

    EXPECT_EQ(expectedBufferingQ, actualBufferingQ);
}