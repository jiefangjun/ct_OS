#include "debug.h"
#include "gdt.h"
#include "console.h"

int kern_entry()
{	
	init_debug();
	init_gdt();
	init_idt();

	console_clear();
	
	printk_color(rc_black, rc_green, "Hello, ct_OS kernel!\n");

	//panic("test");

	asm volatile("int $0x3");
	asm volatile("int $0x4");

	return 0;
}
