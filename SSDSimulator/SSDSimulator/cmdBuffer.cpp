#include <iostream>
#include <sstream>
#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"
#include "ssdCmdParser.h"
#include "IOManager.h"

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
		if (false == isValidFileName(line, fileIdx)) break;
		if (line.substr(2) == "empty") break;

		auto cmd = parser.getCommand(splitByUnderBar(line.substr(2)));
		auto bufferedInfo = cmd->getBufferedCmdInfo();
		if (nullptr == bufferedInfo) break;

		result.push_back(bufferedInfo);
		fileIdx++;
	}

	if (fileIdx <= 5 && false == checkRemainIsEmpty(fileNames, fileIdx)) {
		ioManager.forceCreateFiveFreshBufferFiles();
		return {};
	}

	return result;
}


bool CommandBufferStorage::isValidFileName(std::string& line, int fileIdx)
{
	return line.size() >= 2 &&
		line[0] == ('0' + fileIdx) && line[1] == '_';
}

bool CommandBufferStorage::checkRemainIsEmpty(vector<std::string> fileNames, int fileIdx) {
	vector<std::string> subVector { fileNames.begin() + fileIdx - 1, fileNames.end() };
	for (std::string line : subVector) {
		if (false == isValidFileName(line, fileIdx)) return false;
		if (line.substr(2) != "empty") return false;
		fileIdx++;
	}
	return true;
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