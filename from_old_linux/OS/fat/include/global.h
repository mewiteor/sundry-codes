#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 1
#ifdef	GLOBAL_VARIABLES_HERE
#undef	EXTERN
#define EXTERN
#endif

#include"type.h"
#include"protect.h"

EXTERN u8			gdt_ptr[6];
EXTERN DESCRIPTOR	gdt[GDT_SIZE];
EXTERN u8			idt_ptr[6];
EXTERN GATE			idt[IDT_SIZE];

#endif // _GLOBAL_H_
