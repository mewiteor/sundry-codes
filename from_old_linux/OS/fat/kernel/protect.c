#include "const.h"
#include "type.h"
#include "mlibs.h"
#include "protect.h"
#include "global.h"

PRIVATE void init_idt_desc(u8 vector,u8 desc_type,int_handler handler,u8 privilege);
PRIVATE void init_8259A();

void	divide_error();
void	sigle_step_exception();
void	nmi();
void	breakpoint_exception();
void	overflow();
void	bounds_check();
void	inval_opcode();
void	copr_not_available();
void	double_fault();
void	copr_seg_overrun();
void	inval_tss();
void	segment_not_present();
void	stack_exception();
void	general_protection();
void	page_fault();
void	copr_error();

void init_prot()
{
	init_8259A();

	init_idt_desc(INT_VECTOR_DIVIDE,		DA_386IGate, divide_error,			PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_DEBUG,			DA_386IGate, sigle_step_exception,	PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_NMI,			DA_386IGate, nmi,					PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_BREAKPOINT,	DA_386IGate, breakpoint_exception,	PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_OVERFLOW,		DA_386IGate, overflow,				PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_BOUNDS,		DA_386IGate, bounds_check,			PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_INVAL_OP,		DA_386IGate, inval_opcode,			PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_NOT,	DA_386IGate, copr_not_available,	PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_DOUBLE_FAULT,	DA_386IGate, double_fault,			PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_SEG,	DA_386IGate, copr_seg_overrun,		PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_INVAL_TSS,		DA_386IGate, inval_tss,				PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_SEG_NOT,		DA_386IGate, segment_not_present,	PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_STACK_FAULT,	DA_386IGate, stack_exception,		PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_PROTECTION,	DA_386IGate, general_protection,	PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_PAGE_FAULT,	DA_386IGate, page_fault,			PRIVILEGE_KRNL);
	init_idt_desc(INT_VECTOR_COPROC_ERR,	DA_386IGate, copr_error,			PRIVILEGE_KRNL);
}

void init_idt_desc(u8 vector,u8 desc_type,int_handler handler,u8 privilege)
{
	GATE*	p_gate	= &idt[vector];
	u32		base	= (u32)handler;
	p_gate->offset_low	= base & 0xffff;
	p_gate->selector	= SELECTOR_KERNEL_CS;
	p_gate->dcount		= 0;
	p_gate->attr		= desc_type | (privilege<<5);
	p_gate->offset_high	= base>>16;
}

void init_8259A()
{
	// 主8259,ICW1
	out_byte(INT_M_CTL,		0x11);
	// 从8259,ICW1
	out_byte(INT_S_CTL,		0x11);
	// 主8259,ICW2,设置'主8259'的中断入口地址为0x20
	out_byte(INT_M_CTLMASK,	INT_VECTOR_IRQ0);
	// 从8259,ICW2,设置'从8259'的中断入口地址为0x28
	out_byte(INT_S_CTLMASK,	INT_VECTOR_IRQ8);
	// 主8259,ICW3,IR2对应'从8259'
	out_byte(INT_M_CTLMASK,	4);
	// 从8259,ICW3,对应'主8259'的IR2
	out_byte(INT_S_CTLMASK,	2);
	// 主8259,ICW4
	out_byte(INT_M_CTLMASK,	1);
	// 从8259,ICW4
	out_byte(INT_S_CTLMASK,	1);
	// 主8259,OCW1
	out_byte(INT_M_CTLMASK,	0xff);
	// 从8259,OCW1
	out_byte(INT_S_CTLMASK,	0xff);
}

void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags)
{
	int i;
	int text_color = 0x74;	//	灰底红字

	char *err_msg[] =
	{
		"#DE Divede Error",
		"#DB RESERVED",
		"--  NMI Interrupt",
		"#BP Breakpoint",
		"#OF Overflow",
		"#BR BOUND Range Exceeded",
		"#UD Invalid Opcode (Undefined Opcode)",
		"#NM Device Not Available (NO Math Coprocessor)",
		"#DF Double Fault",
		"    Coprocessor Segment Overrun (reserved)",
		"#TS Invalid Tss",
		"#NP Segment Not Present",
		"#SS Stack-Segment Fault",
		"#GP General Protection",
		"#PF Page Fault",
		"--  (Intel reserved. Do not use.)",
		"#MF x87 FPU Floating-Point Error (Math Fault)",
		"#AC Alignment Check",
		"#MC Machine Check",
		"#XF SIMD Floating-Point Exception"
	};
	cls(0,0,-1,5);
	gotoxy(0,0);

	disp_color_str("Exception! --> ",0x74);
	disp_color_str(err_msg[vec_no],0x74);
	disp_color_str("\n\nEFLAGS:",0x74);
	disp_int(eflags);
	disp_color_str("CS:",0x74);
	disp_int(cs);
	disp_color_str("EIP:",0x74);
	disp_int(eip);

	if(err_code!=-1)
	{
		disp_color_str("Error code:",0x74);
		disp_int(err_code);
	}
}

