#include "pmm.h"
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
	//asm volatile("sti");
	
	
	printk("kernel in memory start: 0x%08X\n", kern_start);
	printk("kernel in memory end: 0x%08X\n", kern_end);
	printk("kernel in memory used: %d KB\n\n", (kern_end - kern_start + 1023) / 1024);
	show_memory_map();
	init_pmm();

	printk_color(rc_black, rc_green, "\nThe Count of Physical Memory Page is: %u\n\n", phy_page_count);

	uint32_t allc_addr = NULL;
	printk_color(rc_black, rc_light_brown, "Test Physical Memory Alloc :\n");
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	allc_addr = pmm_alloc_page();
	printk_color(rc_black, rc_light_brown, "Alloc Physical Addr: 0x%08X\n", allc_addr);
	
	return 0;
}
