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
	bool isValidFileName(std::string& line, int fileIdx);
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
	CmdQ_type  popAllBufferToOutstandingQ();
	void filterInvalidWrites(vector<BufferedCmdInfo*>& bufferingQ);
	void ClearBufferingQ();

protected:
	vector<BufferedCmdInfo*> bufferingQ;
	CommandBufferStorage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;

private:
	std::vector<SsdCmdInterface*> GetBufferedReadCmd(BufferedCmdInfo* readCmdInfo);

	void CheckLbaOverlapBothErases(vector<BufferedCmdInfo*>& bufferingQ, vector<BufferedCmdInfo*>& erasesToRemove);
	void CheckLbaOverlapWriteAndErase(vector<BufferedCmdInfo*>& bufferingQ, vector<BufferedCmdInfo*>& writesToRemove);
	void CheckLbaOverlapBothWrites(vector<BufferedCmdInfo*>& bufferingQ);
	void CompareWithOtherErases(vector<BufferedCmdInfo*>::iterator& itCmd, vector<BufferedCmdInfo*>& bufferingQ, uint32_t startFrontEraseAddress, uint32_t endFrontEraseAddress, vector<BufferedCmdInfo*>& erasesToRemove);
	void CompareWithWrites(vector<BufferedCmdInfo*>& bufferingQ, vector<BufferedCmdInfo*>::iterator& itCommand, uint32_t startEraseAddress, uint32_t endEraseAddress, vector<BufferedCmdInfo*>& writesToRemove);
	void removeFromOutstandingQ(vector<BufferedCmdInfo*>& bufferingQ, vector<BufferedCmdInfo*>& erasesToRemove);

	BufferedCmdInfo* CheckLbaOverlap(vector<BufferedCmdInfo*>& bufferingQ, long address);
};

