/*
 *  kernel.ld -- 针对kernel格式所写的链接脚本
 */

ENTRY(start)
SECTIONS
{
	/*段起始位置*/

	/* 提供内核起始位置地址*/
	/* 从3G开始*/
	PROVIDE(kern_start = 0x400100000);
	
	/*未开启分页内核从内存中的起始位置,依然要从这个地方开始*/
	. = 0x100000;
	.init.text :
	{
		*(.init.text)
		. = ALIGN(4096);
	}
	.init.data :
	{
		*(.init.data)
		. = ALIGN(4096);
	}

	. += 0x400000000;
	.text : AT(ADDR(.text) - 0x400000000)
	{
		*(.text)
		. = ALIGN(4096);
	}
	.data : AT(ADDR(.data) - 0x400000000)
	{
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}
	.bss : AT(ADDR(.bss) - 0x400000000)
	{
		*(.bss)
		. = ALIGN(4096);
	}
	.stab : AT(ADDR(.stab) - 0x400000000)
	{
		*(.stab)
		. = ALIGN(4096);
	}
	.stabstr : AT(ADDR(.stabstr) - 0x400000000)
	{
		*(.stabstr)
		. = ALIGN(4096);
	}
	PROVIDE( kern_end = . );

	/DISCARD/ : { *(.comment) *(.eh_frame) }
}

