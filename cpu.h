#pragma once
#include <iostream>

typedef unsigned char uchar;
typedef signed char schar;

#define RAM_SIZE 256
#define ROM_SIZE 4 * 1024

class cpu
{
public:
	static cpu* getInstance();
	bool initialize(const std::string& fileName);

private:
	cpu() = default;
	cpu(const cpu&) = delete;
	cpu& operator=(const cpu&) = delete;
	cpu(const cpu&&) = delete;
	cpu& operator=(const cpu&&) = delete;

	void clear();
	bool readhexfile(const std::string& fileName);
	int getNumberofLines(FILE* fp);
	void readLineData(FILE* fp, int& memoryLocation);
	void readByte(FILE* fp, uchar& ch);
	void clearSpecialCharacters(FILE* fp, uchar& ch);
	uchar asciiToHex(uchar ch);

	uchar PSW_C();		//Carry
	uchar PSW_AC();		//Auxilary Carry
	uchar PSW_F0();		//Flag 0
	uchar PSW_RS1();	//Register bank selection bit 1
	uchar PSW_RS0();	//Register bank selection bit 1
	uchar PSW_OV();		//Overflow
	uchar PSW_P();		//Parity - Set to 1 if A has odd # of 1's; otherwise reset

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
	const uchar pcon	= 0x87;	//power control
	const uchar tcon	= 0x88;	//time/counter control
	const uchar tmod	= 0x89;	//timer/counter mode control
	const uchar tl0		= 0x8A;		//timer 0 LOW byte
	const uchar tl1		= 0x8B;		//timer 1 LOW byte
	const uchar th0		= 0x8C;		//timer 0 HIGH byte
	const uchar th1		= 0x8D;		//timer 1 HIGH byte
	const uchar p1		= 0x90;		//PORT 1 latch
	const uchar scon	= 0x98;	//serial port control
	const uchar sbuf	= 0x99;	//serial port data buffer
	const uchar p2		= 0xA0;		//PORT 2 latch
	const uchar ie		= 0xA8;		//interrupt enable control
	const uchar p3		= 0xB0;		//PORT 3 latch
	const uchar ip		= 0xB8;		//interrupt priority
	const uchar psw		= 0xD0;		//program status word
	const uchar acc		= 0xE0;		//accumulator
	const uchar b		= 0xF0;		//b register for arithmetic

	//Memory
	uchar ram[RAM_SIZE];
	uchar rom[ROM_SIZE];
};

