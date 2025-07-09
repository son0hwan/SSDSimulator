#include "gmock/gmock.h"
#include "testShell.cpp"
#include "ssdExecutor.cpp"

#ifdef _DEBUG
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main() {
	SSDExecutor ssdExecutor;
	TestShell ts(&ssdExecutor);
	ts.run();
}
#endif
