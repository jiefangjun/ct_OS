#ifndef INCLUDE_IDT_H_
#define INCLUDE_IDT_H_

#include "types.h"

// 初始化中断描述符表
void init_idt();

// 中断描述符
typedef
struct idt_entry_t {
	uint16_t base_lo; 	// 中断描述符地址 15 ~ 0 位
	uint16_t sel; 		// 目标代码段描述符选择子
	uint8_t always0; 	// 置 0 段
	uint8_t flags;  	// 一些标志
	uint16_t base_hi; 	// 中断处理函数地址 31 ~ 16 位
} __attribute__((packed)) idt_entry_t;

// IDTR
typedef 
struct idt_ptr_t {
	uint16_t limit; 	// 限长
	uint32_t base; 		// 基址
} __attribute__((packed)) idt_ptr_t;

#endif


