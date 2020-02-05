#include "cpu.h"

cpu* cpu::instance = nullptr;

cpu* cpu::getInstance()
{
	if (instance == nullptr) {
		instance = new cpu();
	}
	return instance;
}

bool cpu::initialize(const std::string& fileName)
{
	clear();

	auto res = readhexfile(fileName);

	return res;
}

void cpu::clear()
{
	memset(ram, 0, RAM_SIZE);
	memset(rom, 0, ROM_SIZE);
}

bool cpu::readhexfile(const std::string& fileName)
{
	FILE* fp = fopen(fileName.c_str(), "r");
	if (fp == nullptr) {
		std::cerr << "Failed to open file " << fileName << std::endl;
		return false;
	}

	fseek(fp, 0, SEEK_END);
	auto fileSize = ftell(fp);
	rewind(fp);
	if (fileSize > ROM_SIZE) {
		std::cerr << "File is too big" << std::endl;
		fclose(fp);
		return false;
	}
	
	auto lines = getNumberofLines(fp);
	if (lines == 0) {
		std::cerr << "Invalid data" << std::endl;
		fclose(fp);
		return false;
	}

	auto currentLine = 0;
	int memoryLocation = 0;
	while (currentLine < lines) {
		readLineData(fp, memoryLocation);
		++currentLine;
	}

	return true;
}

int cpu::getNumberofLines(FILE* fp)
{
	char ch;
	int lines = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == ':') {
			++lines;
		}
	}
	rewind(fp);
	return lines;
}

void cpu::readLineData(FILE* fp, int& memoryLocation)
{
	//Line Data Breakdown
	uchar byte_count;
	uchar address1;
	uchar address2;
	uchar dataType;
	uchar checkSum;

	uchar ch;
	
	readByte(fp, byte_count);
	
	readByte(fp, address1);

	readByte(fp, address2);
	
	readByte(fp, dataType);

	int currentData = 0;
	while (currentData < byte_count) {
		readByte(fp, ch);
		rom[memoryLocation++] = ch;
		currentData++;
	}

	readByte(fp, checkSum);
}

void cpu::readByte(FILE* fp, uchar& ch)
{
	int total_chars_read = 0;
	uchar temp;
	temp = fgetc(fp);
	clearSpecialCharacters(fp, temp);

	ch = asciiToHex(temp);

	temp = fgetc(fp);
	clearSpecialCharacters(fp, temp);

	ch = (ch << 4) | asciiToHex(temp);
}

void cpu::clearSpecialCharacters(FILE* fp, uchar& ch)
{
	while (ch == ':' || ch == '\r' || ch == '\n') {
		ch = fgetc(fp);
	}
}

uchar cpu::asciiToHex(uchar ch)
{
	if (ch >= '0' && ch <= '9') {
		return ch - '0';
	}
	else if (ch >= 'A' && ch <= 'F') {
		return ch - 'A' + 10;
	}
	else if (ch >= 'a' && ch <= 'f') {
		return ch - 'a' + 10;
	}
	return 0;
}

uchar cpu::PSW_C()
{
	return (ram[psw] & 0x08) >> 6;
}

uchar cpu::PSW_AC()
{
	return (ram[psw] & 0x07) >> 5;
}

uchar cpu::PSW_F0()
{
	return (ram[psw] & 0x06) >> 4;
}

uchar cpu::PSW_RS1()
{
	return (ram[psw] & 0x05) >> 3;
}

uchar cpu::PSW_RS0()
{
	return (ram[psw] & 0x04) >> 2;
}

uchar cpu::PSW_OV()
{
	return (ram[psw] & 0x03) >> 1;
}

uchar cpu::PSW_P()
{
	return ram[psw] & 0x01;
}
