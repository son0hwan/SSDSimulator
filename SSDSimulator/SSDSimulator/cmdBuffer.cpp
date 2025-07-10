#include <iostream>
#include <sstream>
#include <unordered_set>
#include "cmdBuffer.h"
#include "bufferedCmdInfo.h"
#include "ssdCmdParser.h"
#include "IOManager.h"
#include <list>

CommandBuffer::CommandBuffer(CommandBufferStorage& newStorage)
	: storage(newStorage) {
	bufferingQ = newStorage.getBufferFromStorage();
}

std::vector<SsdCmdInterface*> CommandBuffer::addBufferAndGetCmdToRun(SsdCmdInterface* newCmd) {
	BufferedCmdInfo* bufferedInfo = newCmd->getBufferedCmdInfo();
	if (nullptr == bufferedInfo) return { newCmd };

	std::vector<SsdCmdInterface*> outstandingQ;

	if (bufferedInfo->type == CT_READ) return GetBufferedReadCmd(bufferedInfo);

	if (bufferedInfo->type == CT_FLUSH) {
		outstandingQ = popAllBufferToOutstandingQ();
		storage.setBufferToStorage(bufferingQ);
		return outstandingQ;
	}
	
	if (bufferingQ.size() >= Q_SIZE_LIMIT_TO_FLUSH) {
		filterInvalidWrites(bufferingQ);
		outstandingQ = popAllBufferToOutstandingQ();
	}

	bufferingQ.push_back(bufferedInfo);
	filterInvalidWrites(bufferingQ);

	storage.setBufferToStorage(bufferingQ);
	return outstandingQ;
}

std::vector<SsdCmdInterface*> CommandBuffer::GetBufferedReadCmd(BufferedCmdInfo* readCmdInfo)
{
	BufferedCmdInfo* overlappedCmd = CheckLbaOverlap(bufferingQ, readCmdInfo->address);
	if (nullptr == overlappedCmd) return { readCmdInfo->getCmd() };

	uint32_t cachedValue = overlappedCmd->getValueFromAddress(readCmdInfo->address);
	return { new SsdCachedReadCmd{readCmdInfo->address, cachedValue, readCmdInfo->getCmd()} };
}

void CommandBuffer::filterInvalidWrites(vector<BufferedCmdInfo*>& outstandingQ) {
	vector<BufferedCmdInfo*> erasesToRemove;
	CheckLbaOverlapBothErases(outstandingQ, erasesToRemove);
	removeFromOutstandingQ(outstandingQ, erasesToRemove);

	vector<BufferedCmdInfo*> writesToRemove;
	CheckLbaOverlapWriteAndErase(outstandingQ, writesToRemove);
	removeFromOutstandingQ(outstandingQ, writesToRemove);

	CheckLbaOverlapBothWrites(outstandingQ);
}

void CommandBuffer::CheckLbaOverlapBothErases(vector<BufferedCmdInfo*>& outstandingQ, vector<BufferedCmdInfo*>& erasesToRemove)
{
	for (auto itCmd = outstandingQ.begin(); itCmd != outstandingQ.end(); ++itCmd) {
		if ((*itCmd)->type != CT_ERASE) continue;

		uint32_t startFrontEraseAddress = (*itCmd)->address;
		uint32_t endFrontEraseAddress = startFrontEraseAddress + (*itCmd)->size - 1;

		CompareWithOtherErases(itCmd, outstandingQ, startFrontEraseAddress, endFrontEraseAddress, erasesToRemove);
	}
}
void CommandBuffer::CompareWithOtherErases(vector<BufferedCmdInfo*>::iterator& itCmd, vector<BufferedCmdInfo*>& outstandingQ, uint32_t startFrontEraseAddress, uint32_t endFrontEraseAddress, vector<BufferedCmdInfo*>& erasesToRemove)
{
	for (auto itNextCmd = std::next(itCmd); itNextCmd != outstandingQ.end(); ++itNextCmd) {
		if ((*itNextCmd)->type != CT_ERASE) continue;

		uint32_t startRearEraseAddress = (*itNextCmd)->address;
		uint32_t endRearEraseAddress = startRearEraseAddress + (*itNextCmd)->size - 1;

		if (startFrontEraseAddress >= startRearEraseAddress && endFrontEraseAddress <= endRearEraseAddress) {
			erasesToRemove.push_back(*itCmd);
			break;
		}
	}
}

void CommandBuffer::CheckLbaOverlapWriteAndErase(vector<BufferedCmdInfo*>& outstandingQ, vector<BufferedCmdInfo*>& writesToRemove)
{
	for (auto itCommand = outstandingQ.begin(); itCommand != outstandingQ.end(); ++itCommand) {
		if ((*itCommand)->type != CT_ERASE) continue;

		uint32_t startEraseAddress = (*itCommand)->address;
		uint32_t endEraseAddress = startEraseAddress + (*itCommand)->size - 1;

		CompareWithWrites(outstandingQ, itCommand, startEraseAddress, endEraseAddress, writesToRemove);
	}
}
void CommandBuffer::CompareWithWrites(vector<BufferedCmdInfo*>& outstandingQ, vector<BufferedCmdInfo*>::iterator& itCommand, uint32_t startEraseAddress, uint32_t endEraseAddress, vector<BufferedCmdInfo*>& writesToRemove)
{
	for (auto itNextCmd = outstandingQ.begin(); itNextCmd != itCommand; ++itNextCmd) {
		if ((*itNextCmd)->type == CT_WRITE) {
			uint32_t writeAddress = (*itNextCmd)->address;
			if (writeAddress >= startEraseAddress && writeAddress <= endEraseAddress) {
				writesToRemove.push_back(*itNextCmd);
			}
		}
	}
}

void CommandBuffer::CheckLbaOverlapBothWrites(vector<BufferedCmdInfo*>& outstandingQ)
{
	std::unordered_set<uint32_t> seenAddresses;
	for (int i = static_cast<int>(outstandingQ.size()) - 1; i >= 0; --i) {
		auto* cmd = outstandingQ[i];
		if (cmd->type == CT_WRITE) {
			uint32_t addr = cmd->address;
			if (seenAddresses.count(addr)) {
				outstandingQ.erase(outstandingQ.begin() + i);
			}
			else {
				seenAddresses.insert(addr);
			}
		}
	}
}

void CommandBuffer::removeFromOutstandingQ(vector<BufferedCmdInfo*>& outstandingQ, vector<BufferedCmdInfo*>& erasesToRemove)
{
	for (auto* cmdToRemove : erasesToRemove) {
		auto it = std::find(outstandingQ.begin(), outstandingQ.end(), cmdToRemove);
		if (it != outstandingQ.end()) {
			outstandingQ.erase(it);
		}
	}
}

BufferedCmdInfo* CommandBuffer::CheckLbaOverlap(vector<BufferedCmdInfo*>& outstandingQ, long address) {
	BufferedCmdInfo* result = nullptr;
	for (auto itCommand = outstandingQ.begin(); itCommand != outstandingQ.end(); ++itCommand) {


		if ((*itCommand)->type == CT_ERASE) {
			uint32_t startFrontEraseAddress = (*itCommand)->address;
			uint32_t endFrontEraseAddress = startFrontEraseAddress + (*itCommand)->size - 1;
			if (startFrontEraseAddress <= address && address <= endFrontEraseAddress) {
				result = *itCommand;
			}
		}
		else if ((*itCommand)->type == CT_WRITE) {
			uint32_t writeAddress = (*itCommand)->address;
			if (writeAddress == (*itCommand)->address) {
				result = *itCommand;
			}
		}
	}
	return result;
}

vector<SsdCmdInterface*> CommandBuffer::popAllBufferToOutstandingQ() {
	std::vector<SsdCmdInterface*> outstandingQ;
	for (auto bufferedInfo : bufferingQ) {
		outstandingQ.push_back(bufferedInfo->getCmd());
		delete(bufferedInfo);
	}
	bufferingQ.clear();
	return outstandingQ;
}

void CommandBuffer::ClearBufferingQ() {
	for (auto* cmd : bufferingQ) {
		delete cmd;
	}
	bufferingQ.clear();
}

/* CommandBufferStorage class functions */

vector<BufferedCmdInfo*> CommandBufferStorage::getBufferFromStorage() {
	vector<BufferedCmdInfo*> result;
	SsdCmdParser parser;
	IOManager ioManager{ 0 };
	bool isErrored;
	vector<std::string> fileNames = ioManager.buffer().getBufferFileList();

	if (fileNames.size() != 5) {
		ioManager.buffer().forceCreateFiveFreshBufferFiles();
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
		ioManager.buffer().forceCreateFiveFreshBufferFiles();
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
	vector<std::string> subVector{ fileNames.begin() + fileIdx - 1, fileNames.end() };
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

void CommandBufferStorage::setBufferToStorage(vector<BufferedCmdInfo*> cmdQ) {
	SsdCmdParser parser;
	std::vector<std::string> buffer;

	uint32_t bufferNum = 1;
	for (auto cmd : cmdQ) {
		std::vector<std::string> cmdInfo = parser.getEncodedString(cmd->getCmd());
		if (cmdInfo.empty()) {
			throw std::exception("improper command info in cmd Q");
		}
		std::string fileName = "";
		std::string indexStr = std::to_string(bufferNum);
		bufferNum++;

		fileName += indexStr + "_" + cmdInfo[0] + "_" + cmdInfo[1] + "_" + cmdInfo[2];
		buffer.push_back(fileName);
	}

	for (uint32_t i = bufferNum; i <= 5; i++) {
		std::string fileName = "";
		std::string indexStr = std::to_string(bufferNum);
		bufferNum++;

		fileName += indexStr + "_" + "empty";
		buffer.push_back(fileName);
	}

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector() };
	ioManager.buffer().updateBufferFiles(buffer);

}