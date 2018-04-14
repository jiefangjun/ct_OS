#include "common.h"

/*
 * %1代表输入，%0代表输出，输出要加"="号，a和dN代表寄存器约束
 * 行与行直接：分隔开
 * 最后一项代表 clobered register 即变动过的寄存器，需要GCC将其恢复原样
 */

//端口写一个字节
inline void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

//端口读一个字节
inline uint8_t inb(uint16_t port)
{
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

//端口读入一个字
inline uint16_t inw(uint16_t port)
{
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

