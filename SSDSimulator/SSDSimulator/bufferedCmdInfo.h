#include "ssdCmdIncludes.h"

enum commandType {
	CT_READ = 1,
	CT_WRITE = 2,
	CT_ERASE = 3,
	CT_FLUSH = 4,
};

class BufferedCmdInfo {
public:
	BufferedCmdInfo(std::shared_ptr<SsdReadCmd> readCmd) : cmd(readCmd) {
		address = readCmd->getAddress();
		isBufferingRequired = false;
		type = CT_READ;
	}

	BufferedCmdInfo(std::shared_ptr<SsdWriteCmd> writeCmd) : cmd(writeCmd) {
		address = writeCmd->getAddress();
		type = CT_WRITE;
		isBufferingRequired = true;
		value = writeCmd->getData();
	}

	BufferedCmdInfo(std::shared_ptr<SsdEraseCmd> eraseCmd) : cmd(eraseCmd) {
		address = eraseCmd->getStartAddress();
		size = eraseCmd->getSize();
		type = CT_ERASE;
		isBufferingRequired = true;
		value = 0;
	}

	BufferedCmdInfo(std::shared_ptr<SsdFlushCmd> flushCmd) : cmd(flushCmd) {
		address = 0;
		type = CT_FLUSH;
		isBufferingRequired = false;
	}

	SsdCmdInterface* getCmd() { return cmd.get(); }

	uint32_t getValueFromAddress(long targetAddress) {
		if (targetAddress < address || address + size <= targetAddress) throw std::out_of_range("error accessing targetAddress");
		return value;
	}

	//make field as needed
	uint32_t address = 0;
	uint32_t size = 1;
	uint32_t value = 0;
	bool isBufferingRequired = false;

	commandType type;
	std::shared_ptr<SsdCmdInterface> cmd;
};