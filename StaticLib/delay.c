#include "delay.h"

void delay(uint32_t ticks) {
	for (int i=0; i<ticks; i++) {
		__NOP();
	}
}

void delay_us(uint32_t us) {
	__ASM volatile (
		"push {r0}\r\n"
		"mov R0, %0\r\n"
		"_loop:\r\n"
			"cmp R0, #0\r\n"
			"beq _exit\r\n"
			"sub R0, R0,#1\r\n"
			"nop\r\n"
			"b _loop\r\n"
		"_exit:\r\n"
		"pop {r0}\r\n"
		:: "r"(9 * us)
	);
}