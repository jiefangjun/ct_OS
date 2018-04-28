MBOOT_HEADER_MAGIC  equ     0x1BADB002

; 0 号位表示所有的引导模块将按页(4KB)边界对齐
MBOOT_PAGE_ALIGN    equ     1 << 0

; 1 号位通过 Multiboot 信息结构的 mem_* 域包括可用内存的信息
; (告诉GRUB把内存空间的信息包含在Multiboot信息结构中)
MBOOT_MEM_INFO      equ     1 << 1    

; 定义我们使用的 Multiboot 的标记
MBOOT_HEADER_FLAGS  equ     MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO

; 域checksum是一个32位的无符号值，当与其他的magic域(也就是magic和flags)
; 相加时，要求其结果必须是32位的无符号值 0 (即magic+flags+checksum = 0)
MBOOT_CHECKSUM      equ     -(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

[BITS 32] ;所有代码以32-bit的方式编译
section .init.text ;代码段从此开始

; 在代码段的起始位置设置符合 Multiboot 规范的标记

dd MBOOT_HEADER_MAGIC ; GRUB会通过这个魔数来判断该映像是否支持
dd MBOOT_HEADER_FLAGS ; GRUB的一些加载时选项
dd MBOOT_CHECKSUM     ; 检测数值

[GLOBAL start]		; 向外部声明内核代码入口，此处提供该声明给链接器
[GLOBAL mboot_ptr_tmp]  ; 全局 struct multiboot * 变量
[EXTERN kern_entry] 	; 声明内核C代码的入口函数

start:
	cli ; 此时还没有设置号保护模式的中断处理，要关闭中断
		; 所以必须关闭中断
	mov [mboot_ptr_tmp], ebx 	; 将 ebx 中存储的指针存入 glb_mboot_ptr 变量
	mov esp, STACK_TOP		; 设置内核栈地址 按照 multiboot规范
	and esp, 0FFFFFFF0H 		; 栈地址按照16字节对齐
	mov ebp, 0			; 帧指针修改为0

	call kern_entry 		; 调用内核入口函数
;-------------------------------------------------------------------------------------

section .init.data
stack: times 1024 db 0 		; 这里开启临时内核栈
STACK_TOP equ $-stack-1 	; 内核栈顶，$ 符指代当前地址

mboot_ptr_tmp: dd 0		; 全局的 multiboot 结构图指针
