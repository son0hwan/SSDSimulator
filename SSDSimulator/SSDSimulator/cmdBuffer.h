#pragma once
#include <deque>
#include <vector>
#include "ssdInterface.h"

using std::vector;
typedef vector<SsdCmdInterface*>CmdQ_type;

class CommandBufferStorage {
public:
	virtual vector<BufferedCmdInfo*> getBufferFromStorage();
	bool isValidFileName(std::string& line, int fileIdx);
	virtual void setBufferToStorage(vector<BufferedCmdInfo*> cmdQ);
private:
	std::vector<std::string> splitByUnderBar(const std::string& str);
	bool checkRemainIsEmpty(vector<std::string> fileNames, int startCheckIdx);
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
	vector<BufferedCmdInfo*> cmdQ;
	CommandBufferStorage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;
};

