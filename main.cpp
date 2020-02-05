#include <iostream>
#include "cpu.h"

int main() {
	cpu::getInstance()->initialize("D:\\Programming\\8051\\Hello\\Objects\\hello.hex");
	return 0;
}
