#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"

CommandBuffer::CommandBuffer(CommandBufferStroage& stroage)
	: storage(storage) {
	cmdQ = storage.getBufferFromStorage();
}

std::vector<SsdCmdInterface*> CommandBuffer::addBufferAndGetCmdToRun(SsdCmdInterface* newCmd) {
	std::vector<SsdCmdInterface*> resultQ;
	BufferedCmdInfo* bufferedInfo = newCmd->getBufferedCmdInfo();
	if (nullptr == bufferedInfo) {
		resultQ.push_back(newCmd);
		return resultQ;
	}

	if (false == bufferedInfo->isBufferingRequired) {
		//
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

CmdQ_type CommandBufferStroage::getBufferFromStorage() { return CmdQ_type{}; }

void CommandBufferStroage::setBufferToStorage(CmdQ_type cmdQ) {}