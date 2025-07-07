#include "ssdCmdWrite.h"



void SsdWriteCmd::run() {
    //std::vector<ReadWriteData> v;

    //for (int i = 0; i <= this->address; i++) {
    //    v.push_back(ReadWriteData{ i, i * 0x30 });
    //}

    //const std::string RAW_DATA = "ssd_nand.txt";
    //const std::string OUTPUT = "ssd_output.txt";

    //v[address].data = this->data;

    ////W 일 경우 : ssd_nand.txt 에 "해당 부분을 변경한 새로운 전체 값" 기록 후 ssd_output.txt 는 빈 파일로 변경.
    //std::ofstream nandDataFile(RAW_DATA); // Open file for writing
    //if (!nandDataFile) {
    //            //std::cerr << "Error opening file for writing: " << RAW_DATA << std::endl;
    //    throw std::exception("error opening file for writing");
    //}


    //for (int addr = 0; addr < v.size(); addr++) {
    //    nandDataFile << std::hex << addr << ";" << std::hex << std::setw(8) << std::setfill('0') << v[addr].data << std::endl;
    //}
    //nandDataFile.close();

    //std::ofstream outputFile(OUTPUT);
    //outputFile.close();
}