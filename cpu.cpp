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
	long cyclesRemaining = MAX_CYCLES_PER_SECOND;
	auto start = std::chrono::high_resolution_clock::now();
	while (cyclesRemaining > 0) {
		auto func = opcodeHandler[rom[pc]];
		++pc;
		(this->*func)();
		cyclesRemaining--;
		cpu::getInstance()->dumpPort1();
	}
	while (true) {
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
		if (duration.count() > 1) {
			break;
		}
	}
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
	return ram[psw] >> 7;
}

uchar cpu::PSW_AC()
{
	return (ram[psw] & 0x40) >> 6;
}

uchar cpu::PSW_F0()
{
	return (ram[psw] & 0x20) >> 5;
}

uchar cpu::PSW_RS1()
{
	return (ram[psw] & 0x10) >> 6;
}

uchar cpu::PSW_RS0()
{
	return (ram[psw] & 0x08) >> 6;
}

uchar cpu::PSW_OV()
{
	return (ram[psw] & 0x04) >> 6;
}

uchar cpu::PSW_P()
{
	return ram[psw] & 0x01;
}

void cpu::setPSW_C(uchar b)
{
	ram[psw] = ram[psw] & ( b << 7);
}

void cpu::setPSW_AC(uchar b)
{
	ram[psw] = ram[psw] & (b << 6);
}

void cpu::setPSW_F0(uchar b)
{
	ram[psw] = ram[psw] & (b << 5);
}

void cpu::setPSW_RS1(uchar b)
{
	ram[psw] = ram[psw] & (b << 4);
}

void cpu::setPSW_RS0(uchar b)
{
	ram[psw] = ram[psw] & (b << 3);
}

void cpu::setPSW_OV(uchar b)
{
	ram[psw] = ram[psw] & (b << 2);
}

void cpu::setPSW_P(uchar b)
{
	ram[psw] = ram[psw] & b;
}

//opcode handlers
void cpu::opcode_00() {}
void cpu::opcode_01() {}
void cpu::opcode_02() {
	ushort addr = get16hex(rom, pc);
	pc = addr;
}
void cpu::opcode_03() {
	auto temp = ram[acc];
	ram[acc] = ram[acc] >> 1;
	temp = temp & 0x7F;
	ram[acc] = ram[acc] | temp;
}
void cpu::opcode_04() {
	ram[acc] += 1;
}
void cpu::opcode_05() {
	ram[rom[pc]] += 1;
	++pc;
}
void cpu::opcode_06() {
	ram[ram[r0]] += 1;
}
void cpu::opcode_07() {
	ram[ram[r1]] += 1;
}
void cpu::opcode_08() {
	ram[r0] += 1;
}
void cpu::opcode_09() {
	ram[r1] += 1;
}
void cpu::opcode_0A() {
	ram[r2] += 1;
}
void cpu::opcode_0B() {
	ram[r3] += 1;
}
void cpu::opcode_0C() {
	ram[r4] += 1;
}
void cpu::opcode_0D() {
	ram[r5] += 1;
}
void cpu::opcode_0E() {
	ram[r6] += 1;
}
void cpu::opcode_0F() {
	ram[r7] += 1;
}
void cpu::opcode_10() {}
void cpu::opcode_11() {}
void cpu::opcode_12() {}
void cpu::opcode_13() {
	auto temp = ram[acc] & 0x01;
	ram[acc] >>= 7;
	ram[acc] = ram[acc] | (PSW_C());
	setPSW_C(temp);
}
void cpu::opcode_14() {
	ram[acc] -= 1;
}
void cpu::opcode_15() {
	ram[rom[pc]] -= 1;
	++pc;
}
void cpu::opcode_16() {
	ram[ram[r0]] -= 1;
}
void cpu::opcode_17() {
	ram[ram[r1]] -= 1;
}
void cpu::opcode_18() {
	ram[r0] -= 1;
}
void cpu::opcode_19() {
	ram[r1] -= 1;
}
void cpu::opcode_1A() {
	ram[r2] -= 1;
}
void cpu::opcode_1B() {
	ram[r3] -= 1;
}
void cpu::opcode_1C() {
	ram[r4] -= 1;
}
void cpu::opcode_1D() {
	ram[r5] -= 1;
}
void cpu::opcode_1E() {
	ram[r6] -= 1;
}
void cpu::opcode_1F() {
	ram[r7] -= 1;
}
void cpu::opcode_20() {}
void cpu::opcode_21() {}
void cpu::opcode_22() {}
void cpu::opcode_23() {
	auto temp = ram[acc] & 0x01;
	ram[acc] <<= 1;
	ram[acc] |= temp;
}
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
void cpu::opcode_42() {
	ram[rom[pc]] |= ram[acc];
	++pc;
}
void cpu::opcode_43() {
	ram[rom[pc]] |= rom[pc+1];
	pc += 2;
}
void cpu::opcode_44() {
	ram[acc] |= rom[pc + 1];
	pc += 1;
}
void cpu::opcode_45() {
	ram[acc] |= ram[rom[pc + 1]];
	pc += 1;
}
void cpu::opcode_46() {
	ram[acc] |= ram[ram[r0]];
}
void cpu::opcode_47() {
	ram[acc] |= ram[ram[r1]];
}
void cpu::opcode_48() {
	ram[acc] |= ram[r0];
}
void cpu::opcode_49() {
	ram[acc] |= ram[r1];
}
void cpu::opcode_4A() {
	ram[acc] |= ram[r2];
}
void cpu::opcode_4B() {
	ram[acc] |= ram[r3];
}
void cpu::opcode_4C() {
	ram[acc] |= ram[r4];
}
void cpu::opcode_4D() {
	ram[acc] |= ram[r5];
}
void cpu::opcode_4E() {
	ram[acc] |= ram[r6];
}
void cpu::opcode_4F() {
	ram[acc] |= ram[r7];
}
void cpu::opcode_50() {}
void cpu::opcode_51() {}
void cpu::opcode_52() {
	ram[rom[pc]] &= ram[acc];
	++pc;
}
void cpu::opcode_53() {
	ram[rom[pc]] &= rom[pc + 1];
	pc += 2;
}
void cpu::opcode_54() {
	ram[acc] &= rom[pc + 1];
	pc += 1;
}
void cpu::opcode_55() {
	ram[acc] &= ram[rom[pc + 1]];
	pc += 1;
}
void cpu::opcode_56() {
	ram[acc] &= ram[ram[r0]];
}
void cpu::opcode_57() {
	ram[acc] &= ram[ram[r1]];
}
void cpu::opcode_58() {
	ram[acc] &= ram[r0];
}
void cpu::opcode_59() {
	ram[acc] &= ram[r1];
}
void cpu::opcode_5A() {
	ram[acc] &= ram[r2];
}
void cpu::opcode_5B() {
	ram[acc] &= ram[r3];
}
void cpu::opcode_5C() {
	ram[acc] &= ram[r4];
}
void cpu::opcode_5D() {
	ram[acc] &= ram[r5];
}
void cpu::opcode_5E() {
	ram[acc] &= ram[r6];
}
void cpu::opcode_5F() {
	ram[acc] &= ram[r7];
}
void cpu::opcode_60() {}
void cpu::opcode_61() {}
void cpu::opcode_62() {
	ram[rom[pc]] ^= ram[acc];
	++pc;
}
void cpu::opcode_63() {
	ram[rom[pc]] ^= rom[pc + 1];
	pc += 2;
}
void cpu::opcode_64() {
	ram[acc] ^= rom[pc + 1];
	pc += 1;
}
void cpu::opcode_65() {
	ram[acc] ^= ram[rom[pc + 1]];
	pc += 1;
}
void cpu::opcode_66() {
	ram[acc] ^= ram[ram[r0]];
}
void cpu::opcode_67() {
	ram[acc] ^= ram[ram[r1]];
}
void cpu::opcode_68() {
	ram[acc] ^= ram[r0];
}
void cpu::opcode_69() {
	ram[acc] ^= ram[r1];
}
void cpu::opcode_6A() {
	ram[acc] ^= ram[r2];
}
void cpu::opcode_6B() {
	ram[acc] ^= ram[r3];
}
void cpu::opcode_6C() {
	ram[acc] ^= ram[r4];
}
void cpu::opcode_6D() {
	ram[acc] ^= ram[r5];
}
void cpu::opcode_6E() {
	ram[acc] ^= ram[r6];
}
void cpu::opcode_6F() {
	ram[acc] ^= ram[r7];
}
void cpu::opcode_70() {}
void cpu::opcode_71() {}
void cpu::opcode_72() {}
void cpu::opcode_73() {}
void cpu::opcode_74() {
	ram[acc] = rom[pc];
	++pc;
}
void cpu::opcode_75() {
	auto direct = rom[pc++];
	ram[direct] = rom[pc++];
}
void cpu::opcode_76() {
	ram[ram[r0]] = rom[pc];
	++pc;
}
void cpu::opcode_77() {
	ram[ram[r1]] = rom[pc];
	++pc;
}
void cpu::opcode_78() {
	ram[r0] = rom[pc++];
}
void cpu::opcode_79() {
	ram[r1] = rom[pc++];
}
void cpu::opcode_7A() {
	ram[r2] = rom[pc++];
}
void cpu::opcode_7B() {
	ram[r3] = rom[pc++];
}
void cpu::opcode_7C() {
	ram[r4] = rom[pc++];
}
void cpu::opcode_7D() {
	ram[r5] = rom[pc++];
}
void cpu::opcode_7E() {
	ram[r6] = rom[pc++];
}
void cpu::opcode_7F() {
	ram[r7] = rom[pc++];
}
void cpu::opcode_80() {
	pc += 1;
	pc = pc + (schar)rom[pc - 1];
}
void cpu::opcode_81() {}
void cpu::opcode_82() {}
void cpu::opcode_83() {
	++pc;
	ram[acc] = rom[ram[acc] + pc];
}
void cpu::opcode_84() {}
void cpu::opcode_85() {
	ram[pc] = ram[pc + 1];
	pc += 2;
}
void cpu::opcode_86() {
	ram[pc] = ram[ram[r0]];
	pc += 1;
}
void cpu::opcode_87() {
	ram[pc] = ram[ram[r1]];
	pc += 1;
}
void cpu::opcode_88() {
	ram[pc] = ram[r0];
	pc += 1;
}
void cpu::opcode_89() {
	ram[pc] = ram[r1];
	pc += 1;
}
void cpu::opcode_8A() {
	ram[pc] = ram[r2];
	pc += 1;
}
void cpu::opcode_8B() {
	ram[pc] = ram[r3];
	pc += 1;
}
void cpu::opcode_8C() {
	ram[pc] = ram[r4];
	pc += 1;
}
void cpu::opcode_8D() {
	ram[pc] = ram[r5];
	pc += 1;
}
void cpu::opcode_8E() {
	ram[pc] = ram[r6];
	pc += 1;
}
void cpu::opcode_8F() {
	ram[pc] = ram[r7];
	pc += 1;
}
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
void cpu::opcode_A6() {
	ram[ram[r0]] = ram[pc];
	pc += 1;
}
void cpu::opcode_A7() {
	ram[ram[r1]] = ram[pc];
	pc += 1;
}
void cpu::opcode_A8() {
	ram[r0] = ram[pc];
	pc += 1;
}
void cpu::opcode_A9() {
	ram[r1] = ram[pc];
	pc += 1;
}
void cpu::opcode_AA() {
	ram[r2] = ram[pc];
	pc += 1;
}
void cpu::opcode_AB() {
	ram[r3] = ram[pc];
	pc += 1;
}
void cpu::opcode_AC() {
	ram[r4] = ram[pc];
	pc += 1;
}
void cpu::opcode_AD() {
	ram[r5] = ram[pc];
	pc += 1;
}
void cpu::opcode_AE() {
	ram[r6] = ram[pc];
	pc += 1;
}
void cpu::opcode_AF() {
	ram[r7] = ram[pc];
	pc += 1;
}
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
void cpu::opcode_C5() {
	auto temp = ram[acc];
	ram[acc] = ram[pc];
	ram[pc] = temp;
}
void cpu::opcode_C6() {
	auto temp = ram[acc];
	ram[acc] = ram[ram[r0]];
	ram[ram[r0]] = temp;
}
void cpu::opcode_C7() {
	auto temp = ram[acc];
	ram[acc] = ram[ram[r1]];
	ram[ram[r1]] = temp;
}
void cpu::opcode_C8() {
	auto temp = ram[acc];
	ram[acc] = ram[r0];
	ram[r0] = temp;
}
void cpu::opcode_C9() {
	auto temp = ram[acc];
	ram[acc] = ram[r1];
	ram[r1] = temp;
}
void cpu::opcode_CA() {
	auto temp = ram[acc];
	ram[acc] = ram[r2];
	ram[r2] = temp;
}
void cpu::opcode_CB() {
	auto temp = ram[acc];
	ram[acc] = ram[r3];
	ram[r3] = temp;
}
void cpu::opcode_CC() {
	auto temp = ram[acc];
	ram[acc] = ram[r4];
	ram[r4] = temp;
}
void cpu::opcode_CD() {
	auto temp = ram[acc];
	ram[acc] = ram[r5];
	ram[r5] = temp;
}
void cpu::opcode_CE() {
	auto temp = ram[acc];
	ram[acc] = ram[r6];
	ram[r6] = temp;
}
void cpu::opcode_CF() {
	auto temp = ram[acc];
	ram[acc] = ram[r7];
	ram[r7] = temp;
}
void cpu::opcode_D0() {}
void cpu::opcode_D1() {}
void cpu::opcode_D2() {}
void cpu::opcode_D3() {}
void cpu::opcode_D4() {}
void cpu::opcode_D5() {}
void cpu::opcode_D6() {
	auto temp = ram[acc];
	auto temp1 = ram[ram[r0]];
	temp &= 0x0F;
	temp1 &= 0x0F;
	ram[acc] &= 0x0F;
	ram[ram[r0]] &= 0x0F;
	ram[acc] |= temp1;
	ram[ram[r0]] |= temp;
}
void cpu::opcode_D7() {
	auto temp = ram[acc];
	auto temp1 = ram[ram[r1]];
	temp &= 0x0F;
	temp1 &= 0x0F;
	ram[acc] &= 0x0F;
	ram[ram[r1]] &= 0x0F;
	ram[acc] |= temp1;
	ram[ram[r1]] |= temp;
}
void cpu::opcode_D8() {
	ram[r0] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r0] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_D9() {
	ram[r1] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r1] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DA() {
	ram[r2] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r2] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DB() {
	ram[r3] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r3] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DC() {
	ram[r4] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r4] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DD() {
	ram[r5] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r5] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DE() {
	ram[r6] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r6] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_DF() {
	ram[r7] -= 1;
	schar offset = (schar)rom[pc++];
	if (ram[r7] != 0) {
		pc = pc + offset;
	}
}
void cpu::opcode_E0() {}
void cpu::opcode_E1() {}
void cpu::opcode_E2() {}
void cpu::opcode_E3() {}
void cpu::opcode_E4() {
	ram[acc] = 0x00;
}
void cpu::opcode_E5() {
	ram[acc] = ram[pc];
	++pc;
}
void cpu::opcode_E6() {
	ram[acc] = ram[ram[r0]];
}
void cpu::opcode_E7() {
	ram[acc] = ram[ram[r1]];
}
void cpu::opcode_E8() {
	ram[acc] = ram[r0];
}
void cpu::opcode_E9() {
	ram[acc] = ram[r1];
}
void cpu::opcode_EA() {
	ram[acc] = ram[r2];
}
void cpu::opcode_EB() {
	ram[acc] = ram[r3];
}
void cpu::opcode_EC() {
	ram[acc] = ram[r4];
}
void cpu::opcode_ED() {
	ram[acc] = ram[r5];
}
void cpu::opcode_EE() {
	ram[acc] = ram[r6];
}
void cpu::opcode_EF() {
	ram[acc] = ram[r7];
}
void cpu::opcode_F0() {}
void cpu::opcode_F1() {}
void cpu::opcode_F2() {}
void cpu::opcode_F3() {}
void cpu::opcode_F4() {}
void cpu::opcode_F5() {
	ram[pc] = ram[acc];
	++pc;
}
void cpu::opcode_F6() {
	ram[ram[r0]] = ram[acc];
}
void cpu::opcode_F7() {
	ram[ram[r1]] = ram[acc];
}
void cpu::opcode_F8() {
	ram[r0] = ram[acc];
}
void cpu::opcode_F9() {
	ram[r1] = ram[acc];
}
void cpu::opcode_FA() {
	ram[r2] = ram[acc];
}
void cpu::opcode_FB() {
	ram[r3] = ram[acc];
}
void cpu::opcode_FC() {
	ram[r4] = ram[acc];
}
void cpu::opcode_FD() {
	ram[r5] = ram[acc];
}
void cpu::opcode_FE() {
	ram[r6] = ram[acc];
}
void cpu::opcode_FF() {
	ram[r7] = ram[acc];
}
