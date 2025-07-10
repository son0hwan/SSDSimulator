#include "gmock/gmock.h"
#include "shell.cpp"
#include "ssdExecutor.cpp"
#include "consoleInputStrategy.h"
#include "fileInputStrategy.h"

#ifdef _DEBUG
int main(int argc, char* argv[]) {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
    CommandInputStrategy* inputStrategy = nullptr;

    if (argc > 1) {
        inputStrategy = new FileInputStrategy(argv[1]);
    }
    else {
        inputStrategy = new ConsoleInputStrategy();
    }

	SSDExecutor ssdExecutor;
	TestShell ts(&ssdExecutor, inputStrategy);
	ts.run();

    delete inputStrategy;
}
#endif
