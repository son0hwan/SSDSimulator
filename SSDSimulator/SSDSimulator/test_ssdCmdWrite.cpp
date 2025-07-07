#include "gmock/gmock.h"
#include "ssdCmdWrite.h"

TEST(SSD, WriteExecutedWithoutError) {
	long addr = 90;
	long data = 0xADEABB00;
	SsdWriteCmd writeCmd{ addr, data };
	EXPECT_NO_THROW(writeCmd.run());
}

TEST(SSD, WriteErrorInvalidAddressRange) {
	long invalid_addr = 106;
	long data = 0xADEABB00;
	SsdWriteCmd writeCmd{ invalid_addr, data };
	writeCmd.run();

	std::vector<std::string> readFromOutput = writeCmd.TEMPORARY_READ_OUTPUT();

	bool hasError = false;
	for (const auto& l : readFromOutput) {
		if (l.find("ERROR") != std::string::npos) {
			hasError = true;
			break;
		}
	}

	EXPECT_EQ(true, hasError);	
}

TEST(SSD, WriteDataIntegrity) {
	long addr = 90;
	long data = 0xADEABB00;
	SsdWriteCmd writeCmd{ addr, data };
	writeCmd.run();

	writeCmd.TEMPORARY_READ_FROM_SSD_NAND_TXT();

	EXPECT_EQ(data, writeCmd.TEMPORARY_READ_SECTOR_FROM_INTERNAL_BUFFER(addr));
}