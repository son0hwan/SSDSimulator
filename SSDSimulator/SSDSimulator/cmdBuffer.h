#pragma once
#include <deque>
#include <vector>
#include "ssdInterface.h"

using std::vector;
typedef vector<BufferedCmdInfo*> CmdQ_type;

class CommandBufferStroage {
public:
	virtual CmdQ_type getBufferFromStorage();
	virtual void setBufferToStorage(CmdQ_type cmdQ);
};

class CommandBuffer {
public:
	static CommandBuffer& getInstance() {
		static CommandBufferStroage storage{};
		static CommandBuffer instance{ storage };
		return instance;
	}

	CommandBuffer(CommandBufferStroage& stroage);
	vector<SsdCmdInterface*> addBufferAndGetCmdToRun(SsdCmdInterface* newCmd);
	vector<SsdCmdInterface*> popAllBuffer();


protected:
	CmdQ_type cmdQ;
	CommandBufferStroage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;
};

