#include <iostream>
#include <sstream>
#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"
#include "ssdCmdParser.h"
#include "IOManager.h"

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

vector<BufferedCmdInfo*> CommandBufferStorage::getBufferFromStorage() {
	vector<BufferedCmdInfo*> result;
	SsdCmdParser parser;
	IOManager ioManager{ 0 };
	bool isErrored;
	vector<std::string> fileNames = ioManager.getBufferFileList();

	if (fileNames.size() != 5) {
		ioManager.forceCreateFiveFreshBufferFiles();
		return {};
	}

	int fileIdx = 1;
	for (std::string line : fileNames) {
		if (fileNames.size() >= 2 &&
			line[0] == ('0' + fileIdx) && line[1] == '_') {

			if (line.substr(2) == "empty") {
				continue;
			}

			auto cmdString = splitByUnderBar(line.substr(2));

			auto cmd = parser.getCommand(cmdString);

			auto bufferInfo = cmd->getBufferedCmdInfo();
			if (nullptr == bufferInfo) {
				ioManager.forceCreateFiveFreshBufferFiles();
				return {};
			}

			result.push_back(bufferInfo);
		}
		else {
			ioManager.forceCreateFiveFreshBufferFiles();
			return {};
		}

		fileIdx++;
	}

	return result;
}

std::vector<std::string> CommandBufferStorage::splitByUnderBar(const std::string& str) {
	std::stringstream ss{ str };
	std::vector<std::string> results;
	std::string token;

	while (std::getline(ss, token, '_')) {
		results.push_back(token);
	}

	return results;
}

void CommandBufferStorage::setBufferToStorage(vector<BufferedCmdInfo*> cmdQ) {}