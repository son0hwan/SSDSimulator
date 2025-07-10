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
		value = writeCmd->getData();
	}

	BufferedCmdInfo(SsdEraseCmd* eraseCmd) : cmd(eraseCmd) {
		address = eraseCmd->getStartAddress();
		size = eraseCmd->getSize();
		type = CT_ERASE;
		isBufferingRequired = true;
		value = 0;
	}

	BufferedCmdInfo(SsdFlushCmd* flushCmd) : cmd(flushCmd) {
		address = 0;
		type = CT_FLUSH;
		isBufferingRequired = false;
	}

	SsdCmdInterface* getCmd() { return cmd; }

	uint32_t getValueFromAddress(long targetAddress) {
		if (targetAddress < address || address + size <= targetAddress) return 0;
		return value;
	}

	//make field as needed
	uint32_t address = 0;
	uint32_t size = 1;
	uint32_t value = 0;
	bool isBufferingRequired = false;

	commandType type;
	SsdCmdInterface* cmd;
};