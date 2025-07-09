#include "gmock/gmock.h"
#include "testShell.cpp"
#include "ssdExecutor.cpp"

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}