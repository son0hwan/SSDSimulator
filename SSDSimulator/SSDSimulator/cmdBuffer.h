#pragma once
#include <deque>
#include "ssdInterface.h"

using std::deque;
typedef deque<SsdCmdInterface*> CmdQ_type;

class CommandBufferStorage {
public:
	virtual CmdQ_type getBufferFromStorage();
	virtual void setBufferToStorage(CmdQ_type cmdQ);
};

class CommandBuffer {
public:
	static CommandBuffer& getInstance() {
		static CommandBufferStorage storage{};
		static CommandBuffer instance{ storage };
		return instance;
	}

	CommandBuffer(CommandBufferStorage& stroage);
	CmdQ_type addBufferAndGetCmdToRun(SsdCmdInterface* newCmd);
	CmdQ_type popAllBuffer();


protected:
	CmdQ_type cmdQ;
	CommandBufferStorage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;
};

