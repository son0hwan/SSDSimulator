#include "ssdCmdIncludes.h"

class BufferedCmdInfo {
public:

	BufferedCmdInfo(SsdReadCmd* readCmd) : cmd(readCmd) {
		address = readCmd->getAddress();
		///...
		isBufferingRequired = false;
	}

	BufferedCmdInfo(SsdWriteCmd* writeCmd) : cmd(writeCmd) {
		address = writeCmd->getAddress();
		///...
		isBufferingRequired = true;
	}

	BufferedCmdInfo(SsdEraseCmd* eraseCmd) : cmd(eraseCmd) {
		address = eraseCmd->getStartAddress();
		///...
		isBufferingRequired = true;
	}

	BufferedCmdInfo(SsdFlushCmd* flushCmd) : cmd(flushCmd) {
		address = 0;
		///...
		isBufferingRequired = false;
		isFlush = true;
	}

	SsdCmdInterface* getCmd() { return cmd; }

	//make field as needed
	long address = 0;
	long size = 1;
	long valueToWrite = 0;
	bool isBufferingRequired = false;
	bool isFlush = false;
	bool isRead;

	SsdCmdInterface* cmd;

};