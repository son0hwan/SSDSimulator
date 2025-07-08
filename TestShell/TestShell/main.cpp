#include "gmock/gmock.h"
#include "testShell.cpp"
#include "ssdExecutor.cpp"

int main() {
	SSDExecutor ssdExecutor;
	TestShell ts(&ssdExecutor);
	ts.run();

	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}