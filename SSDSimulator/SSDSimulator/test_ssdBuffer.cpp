#include "gmock/gmock.h"
#include "IOManager.h"
#include "ssdSimulator.h"
#include <vector>

#include "cmdBuffer.h"

using namespace testing;

TEST(BufferIOTest, FiveBuffersCreated) {
	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	bool created = ioManager.buffer().forceCreateFiveFreshBufferFiles();
	EXPECT_EQ(true, created);
	std::vector<std::string> createdBuffer = ioManager.buffer().getBufferFileList();
	std::vector<std::string> expectedBuffer = { "1_empty", "2_empty", "3_empty", "4_empty", "5_empty" };
	EXPECT_EQ(createdBuffer, expectedBuffer);
}

TEST(BufferIOTest, RenameBuffer) {
	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	std::vector<std::string> expectedBuffer = { "1_empty", "2_notempty", "3_empty", "4_notempty", "5_notempty" };
	bool created = ioManager.buffer().updateBufferFiles(expectedBuffer);
	EXPECT_EQ(true, created);
	std::vector<std::string> createdBuffer = ioManager.buffer().getBufferFileList();
	EXPECT_EQ(createdBuffer, expectedBuffer);
}

class CommonBufferStorageFixture : public Test {
public:

	void setExistingFileNames(std::vector<std::string> filenames) {
		ioManager.buffer().updateBufferFiles(filenames);
	}

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	CommandBufferStorage storage;
};

TEST_F(CommonBufferStorageFixture, GetValidBuffer) {
	setExistingFileNames({ "1_W_1_0x12345678", "2_W_2_0x12345678", "3_E_3_2", "4_W_5_0x12345678", "5_E_6_10" });

	auto result = storage.getBufferFromStorage();
	EXPECT_EQ(result.size(), 5);
}

TEST_F(CommonBufferStorageFixture, GetInvalidBufferSmallFile) {
	setExistingFileNames({ "1_W_2" });

	auto result = storage.getBufferFromStorage();
	EXPECT_EQ(result.size(), 0);
}

TEST_F(CommonBufferStorageFixture, GetInvalidBufferTooManyFile) {
	setExistingFileNames({ "1_E_1_1", "2_E_2_1", "3_E_3_1", "4_E_4_1", "5_E_5_1", "6_E_6_1" });

	auto result = storage.getBufferFromStorage();
	EXPECT_EQ(result.size(), 0);
}

TEST_F(CommonBufferStorageFixture, GetValidBufferEmpty) {
	setExistingFileNames({ "1_E_1_1", "2_E_2_1", "3_empty", "4_empty", "5_empty" });

	auto result = storage.getBufferFromStorage();
	EXPECT_EQ(result.size(), 2);
}

TEST_F(CommonBufferStorageFixture, GetInValidBufferEmpty) {
	setExistingFileNames({ "1_E_1_1", "2_E_2_1", "3_empty", "4_E_4_1", "5_empty" });

	auto result = storage.getBufferFromStorage();
	EXPECT_EQ(result.size(), 0);
}