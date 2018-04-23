#include "timer.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "console.h"

int kern_entry()
{	
	init_debug();
	init_gdt();
	init_idt();

	console_clear();
	
	printk_color(rc_black, rc_green, "Hello, ct_OS kernel!\n");
	
	// 打印内核调用栈
	// panic("test");

	// 未处理的中断
	// asm volatile("int $0x3");
	// asm volatile("int $0x4");

	init_timer(200);
	
	// 开启中断
	asm volatile("sti");
	
	return 0;
}
