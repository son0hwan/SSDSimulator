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
	std::vector<SsdCmdInterface*> outstandingQ;
	BufferedCmdInfo* bufferedInfo = newCmd->getBufferedCmdInfo();

	if (!CheckBufferingCommand(bufferedInfo, outstandingQ, newCmd)) return outstandingQ;
	
	if (bufferingQ.size() >= Q_SIZE_LIMIT_TO_FLUSH) {
		auto tmp = popAllBuffer();  // 새 vector 생성
		outstandingQ.clear();       // 기존 vector clear
		outstandingQ.insert(outstandingQ.end(), tmp.begin(), tmp.end());
	}

	std::vector<SsdCmdInterface*> cleanedQ(outstandingQ.begin(), outstandingQ.end());
	filterInvalidWrites(cleanedQ);
	outstandingQ = cleanedQ;

	bufferingQ.push_back(bufferedInfo);
	// need change buffer file name

	storage.setBufferToStorage(bufferingQ);
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

void CommandBuffer::filterInvalidWrites(std::vector<SsdCmdInterface*>& outstandingQ) {
	// 1. erase 중복 처리 (앞에 있는 erase가 뒤에 포함되면 앞 erase 삭제)
	std::vector<SsdCmdInterface*> erasesToRemove;
	for (auto itCmd = outstandingQ.begin(); itCmd != outstandingQ.end(); ++itCmd) {
		if ((*itCmd)->getBufferedCmdInfo()->type != CT_ERASE) continue;

		uint32_t startFrontEraseAddress = (*itCmd)->getBufferedCmdInfo()->address;
		uint32_t endFrontEraseAddress = startFrontEraseAddress + (*itCmd)->getBufferedCmdInfo()->size - 1;

		for (auto itNextCmd = std::next(itCmd); itNextCmd != outstandingQ.end(); ++itNextCmd) {
			if ((*itNextCmd)->getBufferedCmdInfo()->type != CT_ERASE) continue;

			auto* rearErase = dynamic_cast<SsdEraseCmd*>(*itNextCmd);
			uint32_t startRearEraseAddress = rearErase->getStartAddress();
			uint32_t endRearEraseAddress = startRearEraseAddress + rearErase->getSize() - 1;

			if (startFrontEraseAddress >= startRearEraseAddress && endFrontEraseAddress <= endRearEraseAddress) {
				erasesToRemove.push_back(*itCmd);
				break; // 앞 erase는 하나만 삭제
			}
		}
	}

	for (auto* cmdToRemove : erasesToRemove) {
		auto it = std::find(outstandingQ.begin(), outstandingQ.end(), cmdToRemove);
		if (it != outstandingQ.end()) {
			outstandingQ.erase(it);
		}
	}

	// 2. erase 앞에 있는 write 삭제 (erase 범위에 포함된 write 삭제)
	std::vector<SsdCmdInterface*> writesToRemove;
	for (auto itCommand = outstandingQ.begin(); itCommand != outstandingQ.end(); ++itCommand) {
		if ((*itCommand)->getBufferedCmdInfo()->type != CT_ERASE) continue;

		uint32_t startEraseAddress = (*itCommand)->getBufferedCmdInfo()->address;
		uint32_t endEraseAddress = startEraseAddress + (*itCommand)->getBufferedCmdInfo()->size - 1;

		for (auto itNextCmd = outstandingQ.begin(); itNextCmd != itCommand; ++itNextCmd) {
			if ((*itNextCmd)->getBufferedCmdInfo()->type == CT_WRITE) {
				uint32_t writeAddress = (*itNextCmd)->getBufferedCmdInfo()->address;
				if (writeAddress >= startEraseAddress && writeAddress <= endEraseAddress) {
					writesToRemove.push_back(*itNextCmd);
				}
			}
		}
	}

	for (auto* cmdToRemove : writesToRemove) {
		auto it = std::find(outstandingQ.begin(), outstandingQ.end(), cmdToRemove);
		if (it != outstandingQ.end()) {
			outstandingQ.erase(it);
		}
	}

	// 3. write 중복 주소 처리 (역순 index 순회, 앞쪽 write 삭제)
	std::unordered_set<uint32_t> seenAddresses;
	for (int i = static_cast<int>(outstandingQ.size()) - 1; i >= 0; --i) {
		auto* cmd = outstandingQ[i];
		if (cmd->getBufferedCmdInfo()->type == CT_WRITE) {
			uint32_t addr = cmd->getBufferedCmdInfo()->address;
			if (seenAddresses.count(addr)) {
				outstandingQ.erase(outstandingQ.begin() + i);
			}
			else {
				seenAddresses.insert(addr);
			}
		}
	}
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

void CommandBuffer::ClearBufferingQ() {
	for (auto* cmd : bufferingQ) {
		delete cmd;  // 각각의 포인터가 가리키는 실제 객체 삭제
	}
	bufferingQ.clear();
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

	IOManager ioManager{ SsdSimulator::getInstance().getMaxSector()};
	ioManager.buffer().updateBufferFiles(buffer);

}