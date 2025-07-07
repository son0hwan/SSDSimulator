#include "gmock/gmock.h"
#include "ssdCmdWrite.h"

TEST(SSD, write_executed_without_error) {
	SsdWriteCmd writeCmd{ 90, static_cast<long>(0xADEABB00) };
	EXPECT_NO_THROW(writeCmd.run());
}

TEST(SSD, write_error_invalid_address_range) {
	SsdWriteCmd writeCmd{ 92, static_cast<long>(0xADEABB00) };
	EXPECT_THROW(writeCmd.run(), std::exception);
}

TEST(SSD, write_data_integrity) {
	SsdWriteCmd writeCmd{ 90, static_cast<long>(0xADEABB00) };
	writeCmd.run();
	
#if TO_BE_ADDED_SOON
	long data = readCmd.run(90);
	if (data != 0xADEABB00) {
		FAIL();
	}
#else
	FAIL(); // Intentional fail until read is implemented 
#endif
}