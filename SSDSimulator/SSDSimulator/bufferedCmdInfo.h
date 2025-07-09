#include "ssdCmdIncludes.h"

enum commandType {
	CT_READ = 1,
	CT_WRITE = 2,
	CT_ERASE = 3,
	CT_FLUSH = 4,
};

class BufferedCmdInfo {
public:
	BufferedCmdInfo(SsdReadCmd* readCmd) : cmd(readCmd) {
		address = readCmd->getAddress();
		isBufferingRequired = false;
		type = CT_READ;
	}

	BufferedCmdInfo(SsdWriteCmd* writeCmd) : cmd(writeCmd) {
		address = writeCmd->getAddress();
		type = CT_WRITE;
		isBufferingRequired = true;
	}

	BufferedCmdInfo(SsdEraseCmd* eraseCmd) : cmd(eraseCmd) {
		address = eraseCmd->getStartAddress();
		size = eraseCmd->getSize();
		type = CT_ERASE;
		isBufferingRequired = true;
	}

	BufferedCmdInfo(SsdFlushCmd* flushCmd) : cmd(flushCmd) {
		address = 0;
		type = CT_FLUSH;
		isBufferingRequired = false;
	}

	SsdCmdInterface* getCmd() { return cmd; }

	//make field as needed
	long address = 0;
	long size = 1;
	bool isBufferingRequired = false;

	commandType type;
	SsdCmdInterface* cmd;
};