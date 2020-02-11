#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>

typedef unsigned char uchar;
typedef signed char schar;
typedef unsigned short ushort;

#define RAM_SIZE 256
#define ROM_SIZE 4 * 1024
#define OPCODES_SIZE 256
#define MAX_CYCLES_PER_SECOND 1000000

#define get16hex(mem, idx) (((ushort)(mem[idx] << 8)) | mem[idx + 1])

typedef void callBackForEveryCycle_t(void*);

class cpu
{
public:
	typedef void (cpu::* opcodeHandler_t)();

	static cpu* getInstance();
	bool initialize(const std::string& fileName, callBackForEveryCycle_t callback, void* obj);
	void emulateCycle();
	void dumpPort1();
	void stopEmulation();

	uchar PSW_C();		//Carry
	uchar PSW_AC();		//Auxilary Carry
	uchar PSW_F0();		//Flag 0
	uchar PSW_RS1();	//Register bank selection bit 1
	uchar PSW_RS0();	//Register bank selection bit 1
	uchar PSW_OV();		//Overflow
	uchar PSW_P();		//Parity - Set to 1 if A has odd # of 1's; otherwise reset

	uchar getP0() {
		return ram[p0];
	}
	uchar getP1() {
		return ram[p1];
	}
	uchar getP2() {
		return ram[p2];
	}
	uchar getP3() {
		return ram[p3];
	}

	uchar getR0() {
		return ram[r0];
	}
	uchar getR1() {
		return ram[r1];
	}
	uchar getR2() {
		return ram[r2];
	}
	uchar getR3() {
		return ram[r3];
	}
	uchar getR4() {
		return ram[r4];
	}
	uchar getR5() {
		return ram[r5];
	}
	uchar getR6() {
		return ram[r6];
	}
	uchar getR7() {
		return ram[r7];
	}

	uchar getB() {
		return ram[b];
	}
	uchar getACC() {
		return ram[acc];
	}
	uchar getIP() {
		return ram[ip];
	}
	uchar getIE() {
		return ram[ie];
	}
	uchar getTMOD() {
		return ram[tmod];
	}
	uchar getTCON() {
		return ram[tcon];
	}
	uchar getPCON() {
		return ram[pcon];
	}
	uchar getDPH() {
		return ram[dph];
	}
	uchar getDPL() {
		return ram[dpl];
	}
	uchar getSP() {
		return ram[sp];
	}
	uchar getPSW() {
		return ram[psw];
	}

	ushort getPC() {
		return pc;
	}

	uchar getTH0() {
		return ram[th0];
	}
	uchar getTH1() {
		return ram[th1];
	}
	uchar getTL0() {
		return ram[tl0];
	}
	uchar getTL1() {
		return ram[tl1];
	}

private:
	cpu() = default;
	cpu(const cpu&) = delete;
	cpu& operator=(const cpu&) = delete;
	cpu(const cpu&&) = delete;
	cpu& operator=(const cpu&&) = delete;

	void clear();
	bool readhexfile(const std::string& fileName);
	int getNumberofLines(FILE* fp);
	bool readLineData(FILE* fp, int& memoryLocation);
	void readByte(FILE* fp, uchar& ch);
	void clearSpecialCharacters(FILE* fp, uchar& ch);
	uchar asciiToHex(uchar ch);
	void initOpcodeArray();
	
	void setPSW_C(uchar b);		//Set Carry
	void setPSW_AC(uchar b);		//Set Auxilary Carry
	void setPSW_F0(uchar b);		//Set Flag 0
	void setPSW_RS1(uchar b);		//Set Register bank selection bit 1
	void setPSW_RS0(uchar b);		//Set Register bank selection bit 1
	void setPSW_OV(uchar b);		//Set Overflow
	void setPSW_P(uchar b);		//Set Parity - Set to 1 if A has odd # of 1's; otherwise reset

	ushort getDPTR();
	void setDPTR(ushort a);
//Members
private:
	static cpu* instance;

	//general purpose registers memory map
	const uchar r0 = 0x00;
	const uchar r1 = 0x01;
	const uchar r2 = 0x02;
	const uchar r3 = 0x03;
	const uchar r4 = 0x04;
	const uchar r5 = 0x05;
	const uchar r6 = 0x06;
	const uchar r7 = 0x07;
	const uchar stack_start = 0x08;
	const uchar bit_addressable_area = 0x20;
	const uchar scratch_pad_start = 0x30;

	//special function regiesters (SFR) memory map
	const uchar p0		= 0x80;		//PORT 0 latch
	const uchar sp		= 0x81;		//stack pointer
	const uchar dpl		= 0x82;		//addressing external memory
	const uchar dph		= 0x83;		//addressing external memory
	const uchar pcon	= 0x87;		//power control
	const uchar tcon	= 0x88;		//time/counter control
	const uchar tmod	= 0x89;		//timer/counter mode control
	const uchar tl0		= 0x8A;		//timer 0 LOW byte
	const uchar tl1		= 0x8B;		//timer 1 LOW byte
	const uchar th0		= 0x8C;		//timer 0 HIGH byte
	const uchar th1		= 0x8D;		//timer 1 HIGH byte
	const uchar p1		= 0x90;		//PORT 1 latch
	const uchar scon	= 0x98;		//serial port control
	const uchar sbuf	= 0x99;		//serial port data buffer
	const uchar p2		= 0xA0;		//PORT 2 latch
	const uchar ie		= 0xA8;		//interrupt enable control
	const uchar p3		= 0xB0;		//PORT 3 latch
	const uchar ip		= 0xB8;		//interrupt priority
	const uchar psw		= 0xD0;		//program status word
	const uchar acc		= 0xE0;		//accumulator
	const uchar b		= 0xF0;		//b register for arithmetic

	//opcode handlers
	std::vector<opcodeHandler_t> opcodeHandler;
	void opcode_00();
	void opcode_01();
	void opcode_02();
	void opcode_03();
	void opcode_04();
	void opcode_05();
	void opcode_06();
	void opcode_07();
	void opcode_08();
	void opcode_09();
	void opcode_0A();
	void opcode_0B();
	void opcode_0C();
	void opcode_0D();
	void opcode_0E();
	void opcode_0F();
	void opcode_10();
	void opcode_11();
	void opcode_12();
	void opcode_13();
	void opcode_14();
	void opcode_15();
	void opcode_16();
	void opcode_17();
	void opcode_18();
	void opcode_19();
	void opcode_1A();
	void opcode_1B();
	void opcode_1C();
	void opcode_1D();
	void opcode_1E();
	void opcode_1F();
	void opcode_20();
	void opcode_21();
	void opcode_22();
	void opcode_23();
	void opcode_24();
	void opcode_25();
	void opcode_26();
	void opcode_27();
	void opcode_28();
	void opcode_29();
	void opcode_2A();
	void opcode_2B();
	void opcode_2C();
	void opcode_2D();
	void opcode_2E();
	void opcode_2F();
	void opcode_30();
	void opcode_31();
	void opcode_32();
	void opcode_33();
	void opcode_34();
	void opcode_35();
	void opcode_36();
	void opcode_37();
	void opcode_38();
	void opcode_39();
	void opcode_3A();
	void opcode_3B();
	void opcode_3C();
	void opcode_3D();
	void opcode_3E();
	void opcode_3F();
	void opcode_40();
	void opcode_41();
	void opcode_42();
	void opcode_43();
	void opcode_44();
	void opcode_45();
	void opcode_46();
	void opcode_47();
	void opcode_48();
	void opcode_49();
	void opcode_4A();
	void opcode_4B();
	void opcode_4C();
	void opcode_4D();
	void opcode_4E();
	void opcode_4F();
	void opcode_50();
	void opcode_51();
	void opcode_52();
	void opcode_53();
	void opcode_54();
	void opcode_55();
	void opcode_56();
	void opcode_57();
	void opcode_58();
	void opcode_59();
	void opcode_5A();
	void opcode_5B();
	void opcode_5C();
	void opcode_5D();
	void opcode_5E();
	void opcode_5F();
	void opcode_60();
	void opcode_61();
	void opcode_62();
	void opcode_63();
	void opcode_64();
	void opcode_65();
	void opcode_66();
	void opcode_67();
	void opcode_68();
	void opcode_69();
	void opcode_6A();
	void opcode_6B();
	void opcode_6C();
	void opcode_6D();
	void opcode_6E();
	void opcode_6F();
	void opcode_70();
	void opcode_71();
	void opcode_72();
	void opcode_73();
	void opcode_74();
	void opcode_75();
	void opcode_76();
	void opcode_77();
	void opcode_78();
	void opcode_79();
	void opcode_7A();
	void opcode_7B();
	void opcode_7C();
	void opcode_7D();
	void opcode_7E();
	void opcode_7F();
	void opcode_80();
	void opcode_81();
	void opcode_82();
	void opcode_83();
	void opcode_84();
	void opcode_85();
	void opcode_86();
	void opcode_87();
	void opcode_88();
	void opcode_89();
	void opcode_8A();
	void opcode_8B();
	void opcode_8C();
	void opcode_8D();
	void opcode_8E();
	void opcode_8F();
	void opcode_90();
	void opcode_91();
	void opcode_92();
	void opcode_93();
	void opcode_94();
	void opcode_95();
	void opcode_96();
	void opcode_97();
	void opcode_98();
	void opcode_99();
	void opcode_9A();
	void opcode_9B();
	void opcode_9C();
	void opcode_9D();
	void opcode_9E();
	void opcode_9F();
	void opcode_A0();
	void opcode_A1();
	void opcode_A2();
	void opcode_A3();
	void opcode_A4();
	void opcode_A5();
	void opcode_A6();
	void opcode_A7();
	void opcode_A8();
	void opcode_A9();
	void opcode_AA();
	void opcode_AB();
	void opcode_AC();
	void opcode_AD();
	void opcode_AE();
	void opcode_AF();
	void opcode_B0();
	void opcode_B1();
	void opcode_B2();
	void opcode_B3();
	void opcode_B4();
	void opcode_B5();
	void opcode_B6();
	void opcode_B7();
	void opcode_B8();
	void opcode_B9();
	void opcode_BA();
	void opcode_BB();
	void opcode_BC();
	void opcode_BD();
	void opcode_BE();
	void opcode_BF();
	void opcode_C0();
	void opcode_C1();
	void opcode_C2();
	void opcode_C3();
	void opcode_C4();
	void opcode_C5();
	void opcode_C6();
	void opcode_C7();
	void opcode_C8();
	void opcode_C9();
	void opcode_CA();
	void opcode_CB();
	void opcode_CC();
	void opcode_CD();
	void opcode_CE();
	void opcode_CF();
	void opcode_D0();
	void opcode_D1();
	void opcode_D2();
	void opcode_D3();
	void opcode_D4();
	void opcode_D5();
	void opcode_D6();
	void opcode_D7();
	void opcode_D8();
	void opcode_D9();
	void opcode_DA();
	void opcode_DB();
	void opcode_DC();
	void opcode_DD();
	void opcode_DE();
	void opcode_DF();
	void opcode_E0();
	void opcode_E1();
	void opcode_E2();
	void opcode_E3();
	void opcode_E4();
	void opcode_E5();
	void opcode_E6();
	void opcode_E7();
	void opcode_E8();
	void opcode_E9();
	void opcode_EA();
	void opcode_EB();
	void opcode_EC();
	void opcode_ED();
	void opcode_EE();
	void opcode_EF();
	void opcode_F0();
	void opcode_F1();
	void opcode_F2();
	void opcode_F3();
	void opcode_F4();
	void opcode_F5();
	void opcode_F6();
	void opcode_F7();
	void opcode_F8();
	void opcode_F9();
	void opcode_FA();
	void opcode_FB();
	void opcode_FC();
	void opcode_FD();
	void opcode_FE();
	void opcode_FF();

	//Memory
	uchar ram[RAM_SIZE];
	uchar rom[ROM_SIZE];
	ushort pc;

	//Callback
	void* callbackFunc;
	void* client;
	bool stop = false;
};

