#pragma once
#include <deque>
#include <vector>
#include "ssdInterface.h"

using std::vector;
typedef vector<SsdCmdInterface*>CmdQ_type;

class CommandBufferStorage {
public:
	virtual vector<BufferedCmdInfo*> getBufferFromStorage();
	virtual void setBufferToStorage(vector<BufferedCmdInfo*> cmdQ);
};

class CommandBuffer {
public:
	static CommandBuffer& getInstance() {
		static CommandBufferStorage storage{};
		static CommandBuffer instance{ storage };
		return instance;
	}

	CommandBuffer(CommandBufferStorage& stroage);
	CmdQ_type  addBufferAndGetCmdToRun(SsdCmdInterface* newCmd);
	CmdQ_type  popAllBuffer();


protected:
	vector<BufferedCmdInfo*> bufferingQ;
	CommandBufferStorage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;

private:
	bool CheckBufferingCommand(BufferedCmdInfo* bufferedInfo, CmdQ_type& resultQ, SsdCmdInterface*& newCmd);
};

