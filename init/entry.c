#include "common.h"
#include "sched.h"
#include "task.h"
#include "heap.h"
#include "vmm.h"
#include "pmm.h"
#include "timer.h"
#include "debug.h"
#include "gdt.h"
#include "idt.h"
#include "console.h"
#include "string.h"

// 内核初始化函数
void kern_init();

// 开启分页机制后的 Multiboot 数据指针
multiboot_t *glb_mboot_ptr;

// 开启分页机制后的内核栈
char kern_stack[STACK_SIZE] __attribute__((aligned(16)));

// 内核栈的栈顶
uint32_t kern_stack_top;

// 内核使用的临时页表和页目录
// 该地址必须是页对其的地址，内存 0-640KB 肯定是空闲的
__attribute__((section(".init.data"))) pgd_t *pgd_tmp = (pgd_t *)0x1000;
__attribute__((section(".init.data"))) pgd_t *pte_low = (pgd_t *)0x2000;
__attribute__((section(".init.data"))) pgd_t *pte_high = (pgd_t *)0x3000;

// 内核入口函数
__attribute__((section(".init.text"))) void kern_entry()
{
	pgd_tmp[0] = (uint32_t)pte_low | PAGE_PRESENT | PAGE_WRITE;
	pgd_tmp[PGD_INDEX(PAGE_OFFSET)] = (uint32_t)pte_high | PAGE_PRESENT | PAGE_WRITE;

	// 映射内核虚拟地址 4MB 到物理地址的前 4MB
	int i;
	for(i = 0; i < 1024; i++) {
		pte_low[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// 映射 0x00000000-0x00400000 的物理地址到虚拟地址 0xC0000000-0xC0400000
	for(i = 0; i < 1024; i++) {
		pte_high[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITE;
	}

	// 设置临时页表
	asm volatile("mov %0, %%cr3" : : "r" (pgd_tmp));

	uint32_t cr0;

	// 启用分页，将 cr0 寄存器的分页位置为 1 就好
	asm volatile("mov %%cr0, %0" : "=r" (cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" : : "r" (cr0));

	// 切换内核栈
	kern_stack_top = ((uint32_t)kern_stack + STACK_SIZE);
	asm volatile("mov %0, %%esp\n\t"
			"xor %%ebp, %%ebp" : : "r" (kern_stack_top));

	// 更新全局 multiboot 指针
	glb_mboot_ptr = mboot_ptr_tmp + PAGE_OFFSET;

	// 调用内核初始化函数
	kern_init();
}

int flag = 0;
int thread(void *arg)
{
	while(1) {
		if (flag == 1) {
			printk_color(rc_black, rc_green, "B");
			flag = 0;
		}
	}
	return 0;
}

void kern_init()
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
	
	
	
	printk("kernel in memory start: 0x%08X\n", kern_start);
	printk("kernel in memory end: 0x%08X\n", kern_end);
	printk("kernel in memory used: %d KB\n\n", (kern_end - kern_start + 1023) / 1024);

	show_memory_map();
	init_pmm();

	init_vmm();
	init_heap();

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

	printk("\n");

	test_heap();

	// 初始化任务调度
	init_sched();

	kernel_thread(thread, NULL);

	// 开启中断
	// enable_intr();

	// 开启中断
	asm volatile("sti");

	while (1) {
		if (flag == 0) {
			printk_color(rc_black, rc_red, "A");
			flag = 1;
		}
	}


	while(1) {
		asm volatile("hlt");
	}
	
}
