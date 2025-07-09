#include "cmdBuffer.h"

CommandBuffer::CommandBuffer(CommandBufferStroage& newStorage)
	: storage(newStorage) {
	cmdQ = newStorage.getBufferFromStorage();
}

CmdQ_type CommandBuffer::addBufferAndGetCmdToRun(SsdCmdInterface* newCmd) {
	CmdQ_type resultQ;
	if (false == newCmd->isAllowBuffering()) {
		//TBD : if buffer read and cached,  ssdBufferReadCmd(value);


		resultQ.push_back(newCmd);
		return resultQ;
	}

	if (cmdQ.size() >= Q_SIZE_LIMIT_TO_FLUSH) {
		resultQ = popAllBuffer();
	}

	cmdQ.push_back(newCmd);



	//storage.setBufferToStorage(cmdQ);
	return resultQ;
}

CmdQ_type CommandBuffer::popAllBuffer() {
	CmdQ_type result = std::move(cmdQ);
	cmdQ = CmdQ_type{};
	return result;
}

CmdQ_type CommandBufferStroage::getBufferFromStorage() { return CmdQ_type{}; }

void CommandBufferStroage::setBufferToStorage(CmdQ_type cmdQ) {}