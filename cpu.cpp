#include "cpu.h"
#include <bitset>

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
	initOpcodeArray();

	ram[sp] = stack_start;
	pc = 0x0000;
	auto res = readhexfile(fileName);

	return res;
}

void cpu::emulateCycle()
{
	(this->*opcodeHandler[rom[pc]])();
}

void cpu::dumpPort1()
{
	std::cout << std::bitset<8>(ram[p1]) << std::endl;
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

	auto lines = getNumberofLines(fp);
	if (lines == 0) {
		std::cerr << "Invalid data" << std::endl;
		fclose(fp);
		return false;
	}

	auto currentLine = 0;
	int memoryLocation = 0;
	while (currentLine < lines) {
		auto res = readLineData(fp, memoryLocation);
		if (!res) {
			std::cerr << "Failed to read data" << std::endl;
			fclose(fp);
			return false;
		}
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

bool cpu::readLineData(FILE* fp, int& memoryLocation)
{
	//Line Data Breakdown
	uchar byte_count;
	uchar address1;
	uchar address2;
	uchar dataType;
	uchar checkSum;
	ushort fulladdress;

	uchar ch;

	readByte(fp, byte_count);

	readByte(fp, address1);

	readByte(fp, address2);

	readByte(fp, dataType);

	fulladdress = address1;
	fulladdress = (fulladdress << 8) | address2;
	if (fulladdress > ROM_SIZE) {
		return false;
	}
	memoryLocation = fulladdress;

	int currentData = 0;
	while (currentData < byte_count) {
		readByte(fp, ch);
		rom[memoryLocation++] = ch;
		currentData++;
	}

	readByte(fp, checkSum);
	return true;
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

void cpu::initOpcodeArray()
{
	opcodeHandler.clear();
	opcodeHandler.reserve(OPCODES_SIZE);
	opcodeHandler.push_back(&cpu::opcode_00);
	opcodeHandler.push_back(&cpu::opcode_01);
	opcodeHandler.push_back(&cpu::opcode_02);
	opcodeHandler.push_back(&cpu::opcode_03);
	opcodeHandler.push_back(&cpu::opcode_04);
	opcodeHandler.push_back(&cpu::opcode_05);
	opcodeHandler.push_back(&cpu::opcode_06);
	opcodeHandler.push_back(&cpu::opcode_07);
	opcodeHandler.push_back(&cpu::opcode_08);
	opcodeHandler.push_back(&cpu::opcode_09);
	opcodeHandler.push_back(&cpu::opcode_0A);
	opcodeHandler.push_back(&cpu::opcode_0B);
	opcodeHandler.push_back(&cpu::opcode_0C);
	opcodeHandler.push_back(&cpu::opcode_0D);
	opcodeHandler.push_back(&cpu::opcode_0E);
	opcodeHandler.push_back(&cpu::opcode_0F);
	opcodeHandler.push_back(&cpu::opcode_10);
	opcodeHandler.push_back(&cpu::opcode_11);
	opcodeHandler.push_back(&cpu::opcode_12);
	opcodeHandler.push_back(&cpu::opcode_13);
	opcodeHandler.push_back(&cpu::opcode_14);
	opcodeHandler.push_back(&cpu::opcode_15);
	opcodeHandler.push_back(&cpu::opcode_16);
	opcodeHandler.push_back(&cpu::opcode_17);
	opcodeHandler.push_back(&cpu::opcode_18);
	opcodeHandler.push_back(&cpu::opcode_19);
	opcodeHandler.push_back(&cpu::opcode_1A);
	opcodeHandler.push_back(&cpu::opcode_1B);
	opcodeHandler.push_back(&cpu::opcode_1C);
	opcodeHandler.push_back(&cpu::opcode_1D);
	opcodeHandler.push_back(&cpu::opcode_1E);
	opcodeHandler.push_back(&cpu::opcode_1F);
	opcodeHandler.push_back(&cpu::opcode_20);
	opcodeHandler.push_back(&cpu::opcode_21);
	opcodeHandler.push_back(&cpu::opcode_22);
	opcodeHandler.push_back(&cpu::opcode_23);
	opcodeHandler.push_back(&cpu::opcode_24);
	opcodeHandler.push_back(&cpu::opcode_25);
	opcodeHandler.push_back(&cpu::opcode_26);
	opcodeHandler.push_back(&cpu::opcode_27);
	opcodeHandler.push_back(&cpu::opcode_28);
	opcodeHandler.push_back(&cpu::opcode_29);
	opcodeHandler.push_back(&cpu::opcode_2A);
	opcodeHandler.push_back(&cpu::opcode_2B);
	opcodeHandler.push_back(&cpu::opcode_2C);
	opcodeHandler.push_back(&cpu::opcode_2D);
	opcodeHandler.push_back(&cpu::opcode_2E);
	opcodeHandler.push_back(&cpu::opcode_2F);
	opcodeHandler.push_back(&cpu::opcode_30);
	opcodeHandler.push_back(&cpu::opcode_31);
	opcodeHandler.push_back(&cpu::opcode_32);
	opcodeHandler.push_back(&cpu::opcode_33);
	opcodeHandler.push_back(&cpu::opcode_34);
	opcodeHandler.push_back(&cpu::opcode_35);
	opcodeHandler.push_back(&cpu::opcode_36);
	opcodeHandler.push_back(&cpu::opcode_37);
	opcodeHandler.push_back(&cpu::opcode_38);
	opcodeHandler.push_back(&cpu::opcode_39);
	opcodeHandler.push_back(&cpu::opcode_3A);
	opcodeHandler.push_back(&cpu::opcode_3B);
	opcodeHandler.push_back(&cpu::opcode_3C);
	opcodeHandler.push_back(&cpu::opcode_3D);
	opcodeHandler.push_back(&cpu::opcode_3E);
	opcodeHandler.push_back(&cpu::opcode_3F);
	opcodeHandler.push_back(&cpu::opcode_40);
	opcodeHandler.push_back(&cpu::opcode_41);
	opcodeHandler.push_back(&cpu::opcode_42);
	opcodeHandler.push_back(&cpu::opcode_43);
	opcodeHandler.push_back(&cpu::opcode_44);
	opcodeHandler.push_back(&cpu::opcode_45);
	opcodeHandler.push_back(&cpu::opcode_46);
	opcodeHandler.push_back(&cpu::opcode_47);
	opcodeHandler.push_back(&cpu::opcode_48);
	opcodeHandler.push_back(&cpu::opcode_49);
	opcodeHandler.push_back(&cpu::opcode_4A);
	opcodeHandler.push_back(&cpu::opcode_4B);
	opcodeHandler.push_back(&cpu::opcode_4C);
	opcodeHandler.push_back(&cpu::opcode_4D);
	opcodeHandler.push_back(&cpu::opcode_4E);
	opcodeHandler.push_back(&cpu::opcode_4F);
	opcodeHandler.push_back(&cpu::opcode_50);
	opcodeHandler.push_back(&cpu::opcode_51);
	opcodeHandler.push_back(&cpu::opcode_52);
	opcodeHandler.push_back(&cpu::opcode_53);
	opcodeHandler.push_back(&cpu::opcode_54);
	opcodeHandler.push_back(&cpu::opcode_55);
	opcodeHandler.push_back(&cpu::opcode_56);
	opcodeHandler.push_back(&cpu::opcode_57);
	opcodeHandler.push_back(&cpu::opcode_58);
	opcodeHandler.push_back(&cpu::opcode_59);
	opcodeHandler.push_back(&cpu::opcode_5A);
	opcodeHandler.push_back(&cpu::opcode_5B);
	opcodeHandler.push_back(&cpu::opcode_5C);
	opcodeHandler.push_back(&cpu::opcode_5D);
	opcodeHandler.push_back(&cpu::opcode_5E);
	opcodeHandler.push_back(&cpu::opcode_5F);
	opcodeHandler.push_back(&cpu::opcode_60);
	opcodeHandler.push_back(&cpu::opcode_61);
	opcodeHandler.push_back(&cpu::opcode_62);
	opcodeHandler.push_back(&cpu::opcode_63);
	opcodeHandler.push_back(&cpu::opcode_64);
	opcodeHandler.push_back(&cpu::opcode_65);
	opcodeHandler.push_back(&cpu::opcode_66);
	opcodeHandler.push_back(&cpu::opcode_67);
	opcodeHandler.push_back(&cpu::opcode_68);
	opcodeHandler.push_back(&cpu::opcode_69);
	opcodeHandler.push_back(&cpu::opcode_6A);
	opcodeHandler.push_back(&cpu::opcode_6B);
	opcodeHandler.push_back(&cpu::opcode_6C);
	opcodeHandler.push_back(&cpu::opcode_6D);
	opcodeHandler.push_back(&cpu::opcode_6E);
	opcodeHandler.push_back(&cpu::opcode_6F);
	opcodeHandler.push_back(&cpu::opcode_70);
	opcodeHandler.push_back(&cpu::opcode_71);
	opcodeHandler.push_back(&cpu::opcode_72);
	opcodeHandler.push_back(&cpu::opcode_73);
	opcodeHandler.push_back(&cpu::opcode_74);
	opcodeHandler.push_back(&cpu::opcode_75);
	opcodeHandler.push_back(&cpu::opcode_76);
	opcodeHandler.push_back(&cpu::opcode_77);
	opcodeHandler.push_back(&cpu::opcode_78);
	opcodeHandler.push_back(&cpu::opcode_79);
	opcodeHandler.push_back(&cpu::opcode_7A);
	opcodeHandler.push_back(&cpu::opcode_7B);
	opcodeHandler.push_back(&cpu::opcode_7C);
	opcodeHandler.push_back(&cpu::opcode_7D);
	opcodeHandler.push_back(&cpu::opcode_7E);
	opcodeHandler.push_back(&cpu::opcode_7F);
	opcodeHandler.push_back(&cpu::opcode_80);
	opcodeHandler.push_back(&cpu::opcode_81);
	opcodeHandler.push_back(&cpu::opcode_82);
	opcodeHandler.push_back(&cpu::opcode_83);
	opcodeHandler.push_back(&cpu::opcode_84);
	opcodeHandler.push_back(&cpu::opcode_85);
	opcodeHandler.push_back(&cpu::opcode_86);
	opcodeHandler.push_back(&cpu::opcode_87);
	opcodeHandler.push_back(&cpu::opcode_88);
	opcodeHandler.push_back(&cpu::opcode_89);
	opcodeHandler.push_back(&cpu::opcode_8A);
	opcodeHandler.push_back(&cpu::opcode_8B);
	opcodeHandler.push_back(&cpu::opcode_8C);
	opcodeHandler.push_back(&cpu::opcode_8D);
	opcodeHandler.push_back(&cpu::opcode_8E);
	opcodeHandler.push_back(&cpu::opcode_8F);
	opcodeHandler.push_back(&cpu::opcode_90);
	opcodeHandler.push_back(&cpu::opcode_91);
	opcodeHandler.push_back(&cpu::opcode_92);
	opcodeHandler.push_back(&cpu::opcode_93);
	opcodeHandler.push_back(&cpu::opcode_94);
	opcodeHandler.push_back(&cpu::opcode_95);
	opcodeHandler.push_back(&cpu::opcode_96);
	opcodeHandler.push_back(&cpu::opcode_97);
	opcodeHandler.push_back(&cpu::opcode_98);
	opcodeHandler.push_back(&cpu::opcode_99);
	opcodeHandler.push_back(&cpu::opcode_9A);
	opcodeHandler.push_back(&cpu::opcode_9B);
	opcodeHandler.push_back(&cpu::opcode_9C);
	opcodeHandler.push_back(&cpu::opcode_9D);
	opcodeHandler.push_back(&cpu::opcode_9E);
	opcodeHandler.push_back(&cpu::opcode_9F);
	opcodeHandler.push_back(&cpu::opcode_A0);
	opcodeHandler.push_back(&cpu::opcode_A1);
	opcodeHandler.push_back(&cpu::opcode_A2);
	opcodeHandler.push_back(&cpu::opcode_A3);
	opcodeHandler.push_back(&cpu::opcode_A4);
	opcodeHandler.push_back(&cpu::opcode_A5);
	opcodeHandler.push_back(&cpu::opcode_A6);
	opcodeHandler.push_back(&cpu::opcode_A7);
	opcodeHandler.push_back(&cpu::opcode_A8);
	opcodeHandler.push_back(&cpu::opcode_A9);
	opcodeHandler.push_back(&cpu::opcode_AA);
	opcodeHandler.push_back(&cpu::opcode_AB);
	opcodeHandler.push_back(&cpu::opcode_AC);
	opcodeHandler.push_back(&cpu::opcode_AD);
	opcodeHandler.push_back(&cpu::opcode_AE);
	opcodeHandler.push_back(&cpu::opcode_AF);
	opcodeHandler.push_back(&cpu::opcode_B0);
	opcodeHandler.push_back(&cpu::opcode_B1);
	opcodeHandler.push_back(&cpu::opcode_B2);
	opcodeHandler.push_back(&cpu::opcode_B3);
	opcodeHandler.push_back(&cpu::opcode_B4);
	opcodeHandler.push_back(&cpu::opcode_B5);
	opcodeHandler.push_back(&cpu::opcode_B6);
	opcodeHandler.push_back(&cpu::opcode_B7);
	opcodeHandler.push_back(&cpu::opcode_B8);
	opcodeHandler.push_back(&cpu::opcode_B9);
	opcodeHandler.push_back(&cpu::opcode_BA);
	opcodeHandler.push_back(&cpu::opcode_BB);
	opcodeHandler.push_back(&cpu::opcode_BC);
	opcodeHandler.push_back(&cpu::opcode_BD);
	opcodeHandler.push_back(&cpu::opcode_BE);
	opcodeHandler.push_back(&cpu::opcode_BF);
	opcodeHandler.push_back(&cpu::opcode_C0);
	opcodeHandler.push_back(&cpu::opcode_C1);
	opcodeHandler.push_back(&cpu::opcode_C2);
	opcodeHandler.push_back(&cpu::opcode_C3);
	opcodeHandler.push_back(&cpu::opcode_C4);
	opcodeHandler.push_back(&cpu::opcode_C5);
	opcodeHandler.push_back(&cpu::opcode_C6);
	opcodeHandler.push_back(&cpu::opcode_C7);
	opcodeHandler.push_back(&cpu::opcode_C8);
	opcodeHandler.push_back(&cpu::opcode_C9);
	opcodeHandler.push_back(&cpu::opcode_CA);
	opcodeHandler.push_back(&cpu::opcode_CB);
	opcodeHandler.push_back(&cpu::opcode_CC);
	opcodeHandler.push_back(&cpu::opcode_CD);
	opcodeHandler.push_back(&cpu::opcode_CE);
	opcodeHandler.push_back(&cpu::opcode_CF);
	opcodeHandler.push_back(&cpu::opcode_D0);
	opcodeHandler.push_back(&cpu::opcode_D1);
	opcodeHandler.push_back(&cpu::opcode_D2);
	opcodeHandler.push_back(&cpu::opcode_D3);
	opcodeHandler.push_back(&cpu::opcode_D4);
	opcodeHandler.push_back(&cpu::opcode_D5);
	opcodeHandler.push_back(&cpu::opcode_D6);
	opcodeHandler.push_back(&cpu::opcode_D7);
	opcodeHandler.push_back(&cpu::opcode_D8);
	opcodeHandler.push_back(&cpu::opcode_D9);
	opcodeHandler.push_back(&cpu::opcode_DA);
	opcodeHandler.push_back(&cpu::opcode_DB);
	opcodeHandler.push_back(&cpu::opcode_DC);
	opcodeHandler.push_back(&cpu::opcode_DD);
	opcodeHandler.push_back(&cpu::opcode_DE);
	opcodeHandler.push_back(&cpu::opcode_DF);
	opcodeHandler.push_back(&cpu::opcode_E0);
	opcodeHandler.push_back(&cpu::opcode_E1);
	opcodeHandler.push_back(&cpu::opcode_E2);
	opcodeHandler.push_back(&cpu::opcode_E3);
	opcodeHandler.push_back(&cpu::opcode_E4);
	opcodeHandler.push_back(&cpu::opcode_E5);
	opcodeHandler.push_back(&cpu::opcode_E6);
	opcodeHandler.push_back(&cpu::opcode_E7);
	opcodeHandler.push_back(&cpu::opcode_E8);
	opcodeHandler.push_back(&cpu::opcode_E9);
	opcodeHandler.push_back(&cpu::opcode_EA);
	opcodeHandler.push_back(&cpu::opcode_EB);
	opcodeHandler.push_back(&cpu::opcode_EC);
	opcodeHandler.push_back(&cpu::opcode_ED);
	opcodeHandler.push_back(&cpu::opcode_EE);
	opcodeHandler.push_back(&cpu::opcode_EF);
	opcodeHandler.push_back(&cpu::opcode_F0);
	opcodeHandler.push_back(&cpu::opcode_F1);
	opcodeHandler.push_back(&cpu::opcode_F2);
	opcodeHandler.push_back(&cpu::opcode_F3);
	opcodeHandler.push_back(&cpu::opcode_F4);
	opcodeHandler.push_back(&cpu::opcode_F5);
	opcodeHandler.push_back(&cpu::opcode_F6);
	opcodeHandler.push_back(&cpu::opcode_F7);
	opcodeHandler.push_back(&cpu::opcode_F8);
	opcodeHandler.push_back(&cpu::opcode_F9);
	opcodeHandler.push_back(&cpu::opcode_FA);
	opcodeHandler.push_back(&cpu::opcode_FB);
	opcodeHandler.push_back(&cpu::opcode_FC);
	opcodeHandler.push_back(&cpu::opcode_FD);
	opcodeHandler.push_back(&cpu::opcode_FE);
	opcodeHandler.push_back(&cpu::opcode_FF);
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

//opcode handlers
void cpu::opcode_00() { ++pc; }
void cpu::opcode_01() {}
void cpu::opcode_02() {
	++pc;
	ushort addr = get16hex(rom, pc);
	pc = addr;
}
void cpu::opcode_03() {}
void cpu::opcode_04() {}
void cpu::opcode_05() {}
void cpu::opcode_06() {}
void cpu::opcode_07() {}
void cpu::opcode_08() {}
void cpu::opcode_09() {}
void cpu::opcode_0A() {}
void cpu::opcode_0B() {}
void cpu::opcode_0C() {}
void cpu::opcode_0D() {}
void cpu::opcode_0E() {}
void cpu::opcode_0F() {}
void cpu::opcode_10() {}
void cpu::opcode_11() {}
void cpu::opcode_12() {}
void cpu::opcode_13() {}
void cpu::opcode_14() {}
void cpu::opcode_15() {}
void cpu::opcode_16() {}
void cpu::opcode_17() {}
void cpu::opcode_18() {}
void cpu::opcode_19() {}
void cpu::opcode_1A() {}
void cpu::opcode_1B() {}
void cpu::opcode_1C() {}
void cpu::opcode_1D() {}
void cpu::opcode_1E() {}
void cpu::opcode_1F() {}
void cpu::opcode_20() {}
void cpu::opcode_21() {}
void cpu::opcode_22() {}
void cpu::opcode_23() {}
void cpu::opcode_24() {}
void cpu::opcode_25() {}
void cpu::opcode_26() {}
void cpu::opcode_27() {}
void cpu::opcode_28() {}
void cpu::opcode_29() {}
void cpu::opcode_2A() {}
void cpu::opcode_2B() {}
void cpu::opcode_2C() {}
void cpu::opcode_2D() {}
void cpu::opcode_2E() {}
void cpu::opcode_2F() {}
void cpu::opcode_30() {}
void cpu::opcode_31() {}
void cpu::opcode_32() {}
void cpu::opcode_33() {}
void cpu::opcode_34() {}
void cpu::opcode_35() {}
void cpu::opcode_36() {}
void cpu::opcode_37() {}
void cpu::opcode_38() {}
void cpu::opcode_39() {}
void cpu::opcode_3A() {}
void cpu::opcode_3B() {}
void cpu::opcode_3C() {}
void cpu::opcode_3D() {}
void cpu::opcode_3E() {}
void cpu::opcode_3F() {}
void cpu::opcode_40() {}
void cpu::opcode_41() {}
void cpu::opcode_42() {}
void cpu::opcode_43() {}
void cpu::opcode_44() {}
void cpu::opcode_45() {}
void cpu::opcode_46() {}
void cpu::opcode_47() {}
void cpu::opcode_48() {}
void cpu::opcode_49() {}
void cpu::opcode_4A() {}
void cpu::opcode_4B() {}
void cpu::opcode_4C() {}
void cpu::opcode_4D() {}
void cpu::opcode_4E() {}
void cpu::opcode_4F() {}
void cpu::opcode_50() {}
void cpu::opcode_51() {}
void cpu::opcode_52() {}
void cpu::opcode_53() {}
void cpu::opcode_54() {}
void cpu::opcode_55() {}
void cpu::opcode_56() {}
void cpu::opcode_57() {}
void cpu::opcode_58() {}
void cpu::opcode_59() {}
void cpu::opcode_5A() {}
void cpu::opcode_5B() {}
void cpu::opcode_5C() {}
void cpu::opcode_5D() {}
void cpu::opcode_5E() {}
void cpu::opcode_5F() {}
void cpu::opcode_60() {}
void cpu::opcode_61() {}
void cpu::opcode_62() {}
void cpu::opcode_63() {
	++pc;
	auto direct = rom[pc++];
	ram[direct] = ram[direct] ^ rom[pc++];
}
void cpu::opcode_64() {}
void cpu::opcode_65() {}
void cpu::opcode_66() {}
void cpu::opcode_67() {}
void cpu::opcode_68() {}
void cpu::opcode_69() {}
void cpu::opcode_6A() {}
void cpu::opcode_6B() {}
void cpu::opcode_6C() {}
void cpu::opcode_6D() {}
void cpu::opcode_6E() {}
void cpu::opcode_6F() {}
void cpu::opcode_70() {}
void cpu::opcode_71() {}
void cpu::opcode_72() {}
void cpu::opcode_73() {}
void cpu::opcode_74() {}
void cpu::opcode_75() {
	++pc;
	auto direct = rom[pc++];
	ram[direct] = rom[pc++];
}
void cpu::opcode_76() {}
void cpu::opcode_77() {}
void cpu::opcode_78() {
	++pc;
	ram[r0] = rom[pc++];
}
void cpu::opcode_79() {}
void cpu::opcode_7A() {}
void cpu::opcode_7B() {}
void cpu::opcode_7C() {}
void cpu::opcode_7D() {}
void cpu::opcode_7E() {}
void cpu::opcode_7F() {}
void cpu::opcode_80() {
	++pc;
	schar offset = (schar)rom[pc++];
	pc = pc + offset;
}
void cpu::opcode_81() {}
void cpu::opcode_82() {}
void cpu::opcode_83() {}
void cpu::opcode_84() {}
void cpu::opcode_85() {}
void cpu::opcode_86() {}
void cpu::opcode_87() {}
void cpu::opcode_88() {}
void cpu::opcode_89() {}
void cpu::opcode_8A() {}
void cpu::opcode_8B() {}
void cpu::opcode_8C() {}
void cpu::opcode_8D() {}
void cpu::opcode_8E() {}
void cpu::opcode_8F() {}
void cpu::opcode_90() {}
void cpu::opcode_91() {}
void cpu::opcode_92() {}
void cpu::opcode_93() {}
void cpu::opcode_94() {}
void cpu::opcode_95() {}
void cpu::opcode_96() {}
void cpu::opcode_97() {}
void cpu::opcode_98() {}
void cpu::opcode_99() {}
void cpu::opcode_9A() {}
void cpu::opcode_9B() {}
void cpu::opcode_9C() {}
void cpu::opcode_9D() {}
void cpu::opcode_9E() {}
void cpu::opcode_9F() {}
void cpu::opcode_A0() {}
void cpu::opcode_A1() {}
void cpu::opcode_A2() {}
void cpu::opcode_A3() {}
void cpu::opcode_A4() {}
void cpu::opcode_A5() {}
void cpu::opcode_A6() {}
void cpu::opcode_A7() {}
void cpu::opcode_A8() {}
void cpu::opcode_A9() {}
void cpu::opcode_AA() {}
void cpu::opcode_AB() {}
void cpu::opcode_AC() {}
void cpu::opcode_AD() {}
void cpu::opcode_AE() {}
void cpu::opcode_AF() {}
void cpu::opcode_B0() {}
void cpu::opcode_B1() {}
void cpu::opcode_B2() {}
void cpu::opcode_B3() {}
void cpu::opcode_B4() {}
void cpu::opcode_B5() {}
void cpu::opcode_B6() {}
void cpu::opcode_B7() {}
void cpu::opcode_B8() {}
void cpu::opcode_B9() {}
void cpu::opcode_BA() {}
void cpu::opcode_BB() {}
void cpu::opcode_BC() {}
void cpu::opcode_BD() {}
void cpu::opcode_BE() {}
void cpu::opcode_BF() {}
void cpu::opcode_C0() {}
void cpu::opcode_C1() {}
void cpu::opcode_C2() {}
void cpu::opcode_C3() {}
void cpu::opcode_C4() {}
void cpu::opcode_C5() {}
void cpu::opcode_C6() {}
void cpu::opcode_C7() {}
void cpu::opcode_C8() {}
void cpu::opcode_C9() {}
void cpu::opcode_CA() {}
void cpu::opcode_CB() {}
void cpu::opcode_CC() {}
void cpu::opcode_CD() {}
void cpu::opcode_CE() {}
void cpu::opcode_CF() {}
void cpu::opcode_D0() {}
void cpu::opcode_D1() {}
void cpu::opcode_D2() {}
void cpu::opcode_D3() {}
void cpu::opcode_D4() {}
void cpu::opcode_D5() {}
void cpu::opcode_D6() {}
void cpu::opcode_D7() {}
void cpu::opcode_D8() {
	++pc;
	schar value = (schar)ram[r0];
	value -= 1;
	schar offset = (schar)rom[pc++];
	if (value != 0) {
		ram[r0] = value;
		pc = pc + offset;
	}
}
void cpu::opcode_D9() {}
void cpu::opcode_DA() {}
void cpu::opcode_DB() {}
void cpu::opcode_DC() {}
void cpu::opcode_DD() {}
void cpu::opcode_DE() {}
void cpu::opcode_DF() {}
void cpu::opcode_E0() {}
void cpu::opcode_E1() {}
void cpu::opcode_E2() {}
void cpu::opcode_E3() {}
void cpu::opcode_E4() {
	ram[acc] = 0x00;
	++pc;
}
void cpu::opcode_E5() {}
void cpu::opcode_E6() {}
void cpu::opcode_E7() {}
void cpu::opcode_E8() {}
void cpu::opcode_E9() {}
void cpu::opcode_EA() {}
void cpu::opcode_EB() {}
void cpu::opcode_EC() {}
void cpu::opcode_ED() {}
void cpu::opcode_EE() {}
void cpu::opcode_EF() {}
void cpu::opcode_F0() {}
void cpu::opcode_F1() {}
void cpu::opcode_F2() {}
void cpu::opcode_F3() {}
void cpu::opcode_F4() {}
void cpu::opcode_F5() {}
void cpu::opcode_F6() {
	ram[ram[r0]] = ram[acc];
	++pc;
}
void cpu::opcode_F7() {}
void cpu::opcode_F8() {}
void cpu::opcode_F9() {}
void cpu::opcode_FA() {}
void cpu::opcode_FB() {}
void cpu::opcode_FC() {}
void cpu::opcode_FD() {}
void cpu::opcode_FE() {}
void cpu::opcode_FF() {}
