#include "debug.h"
#include "console.h"

int kern_entry()
{	
	init_debug();

	console_clear();
	
	printk_color(rc_black, rc_green, "Hello, ct_OS kernel!\n");

	panic("test");

	return 0;
}
