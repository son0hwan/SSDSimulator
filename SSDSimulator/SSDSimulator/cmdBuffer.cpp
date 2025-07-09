#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"

CommandBuffer::CommandBuffer(CommandBufferStorage& newStorage)
	: storage(newStorage) {
	bufferingQ = newStorage.getBufferFromStorage();
}

std::vector<SsdCmdInterface*> CommandBuffer::addBufferAndGetCmdToRun(SsdCmdInterface* newCmd) {
	std::vector<SsdCmdInterface*> outstandingQ;
	BufferedCmdInfo* bufferedInfo = newCmd->getBufferedCmdInfo();

	if (!CheckBufferingCommand(bufferedInfo, outstandingQ, newCmd)) return outstandingQ;
	
	if (bufferingQ.size() >= Q_SIZE_LIMIT_TO_FLUSH) {
		outstandingQ = popAllBuffer();
	}

	bufferingQ.push_back(bufferedInfo);
	// need change buffer file name

	//storage.setBufferToStorage(cmdQ);
	return outstandingQ;
}

bool CommandBuffer::CheckBufferingCommand(BufferedCmdInfo* bufferedInfo, CmdQ_type& outstandingQ, SsdCmdInterface*& newCmd)
{
	if (nullptr == bufferedInfo || false == bufferedInfo->isBufferingRequired) {
		outstandingQ.push_back(newCmd);
		return false;
	}
	return true;
}

vector<SsdCmdInterface*> CommandBuffer::popAllBuffer() {
	std::vector<SsdCmdInterface*> outstandingQ;
	for (auto bufferedInfo : bufferingQ) {
		outstandingQ.push_back(bufferedInfo->getCmd());
		delete(bufferedInfo);
	}
	bufferingQ.clear();
	return outstandingQ;
}

vector<BufferedCmdInfo*> CommandBufferStorage::getBufferFromStorage() { return vector<BufferedCmdInfo*>{}; }

void CommandBufferStorage::setBufferToStorage(vector<BufferedCmdInfo*> cmdQ) {}