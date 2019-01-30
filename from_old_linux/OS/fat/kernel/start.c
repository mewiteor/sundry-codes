#include "protect.h"
#include "mlibs.h"
#include "global.h"

PUBLIC void cstart()
{
	gotoxy(15,0);
	disp_str("-----\"cstart\" begin-----\n");
	memcpy(&gdt,						// 新的GDT
			(void*)*(u32*)&gdt_ptr[2],	// 旧GDT的Base
			*(u16*)gdt_ptr+1		// 旧GDT的Limit
		  );
	*(u16*)gdt_ptr		= GDT_SIZE*sizeof(DESCRIPTOR)-1;
	*(u32*)&gdt_ptr[2]	= (u32)&gdt;
	*(u16*)idt_ptr		= IDT_SIZE*sizeof(GATE)-1;
	*(u32*)&idt_ptr[2]	= (u32)&idt;
	init_prot();
	disp_str("-----\"cstart\" end-----\n");
}
