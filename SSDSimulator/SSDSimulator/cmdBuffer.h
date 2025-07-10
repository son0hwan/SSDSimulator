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
	void filterInvalidWrites(std::vector<SsdCmdInterface*>& outstandingQ);
	void ClearBufferingQ();

protected:
	vector<BufferedCmdInfo*> bufferingQ;
	CommandBufferStorage& storage;

	static const int Q_SIZE_LIMIT_TO_FLUSH = 5;

private:
	bool isBufferingCmd(BufferedCmdInfo* newCmd);
	std::vector<SsdCmdInterface*> GetBufferedReadCmd(BufferedCmdInfo* readCmdInfo);

	void CheckLbaOverlapBothErases(CmdQ_type& outstandingQ, CmdQ_type& erasesToRemove);
	void CheckLbaOverlapWriteAndErase(CmdQ_type& outstandingQ, CmdQ_type& writesToRemove);
	void CheckLbaOverlapBothWrites(CmdQ_type& outstandingQ);
	void CompareWithOtherErases(CmdQ_type::iterator& itCmd, CmdQ_type& outstandingQ, uint32_t startFrontEraseAddress, uint32_t endFrontEraseAddress, CmdQ_type& erasesToRemove);
	void CompareWithWrites(CmdQ_type& outstandingQ, CmdQ_type::iterator& itCommand, uint32_t startEraseAddress, uint32_t endEraseAddress, CmdQ_type& writesToRemove);
	void removeFromOutstandingQ(CmdQ_type& outstandingQ, CmdQ_type& erasesToRemove);

	BufferedCmdInfo* CheckLbaOverlap(vector<BufferedCmdInfo*>& outstandingQ, long address);
};

