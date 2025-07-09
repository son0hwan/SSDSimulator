#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"

CommandBuffer::CommandBuffer(CommandBufferStorage& newStorage)
	: storage(newStorage) {
	cmdQ = newStorage.getBufferFromStorage();
}

std::vector<SsdCmdInterface*> CommandBuffer::addBufferAndGetCmdToRun(SsdCmdInterface* newCmd) {
	std::vector<SsdCmdInterface*> resultQ;
	BufferedCmdInfo* bufferedInfo = newCmd->getBufferedCmdInfo();
	if (nullptr == bufferedInfo) {
		resultQ.push_back(newCmd);
		return resultQ;
	}

	if (false == bufferedInfo->isBufferingRequired) {
		delete(bufferedInfo);
		resultQ.push_back(newCmd);
		return resultQ;
	}

	if (cmdQ.size() >= Q_SIZE_LIMIT_TO_FLUSH) {
		resultQ = popAllBuffer();
	}

	//
	cmdQ.push_back(bufferedInfo);

	//storage.setBufferToStorage(cmdQ);
	return resultQ;
}

vector<SsdCmdInterface*> CommandBuffer::popAllBuffer() {
	std::vector<SsdCmdInterface*> resultQ;
	for (auto bufferedInfo : cmdQ) {
		resultQ.push_back(bufferedInfo->getCmd());
		delete(bufferedInfo);
	}
	cmdQ.clear();
	return resultQ;
}

vector<BufferedCmdInfo*> CommandBufferStorage::getBufferFromStorage() { return vector<BufferedCmdInfo*>{}; }

void CommandBufferStorage::setBufferToStorage(vector<BufferedCmdInfo*> cmdQ) {}