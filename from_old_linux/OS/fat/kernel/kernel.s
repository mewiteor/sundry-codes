.set	SELECTOR_KERNEL_CS,	8

.extern	cstart
.extern	gdt_ptr
.extern idt_ptr

.section .bss
StackSpace:		.space	2048
StackTop:
.section .text
.globl _start
.globl divide_error
.globl sigle_step_exception
.globl nmi
.globl breakpoint_exception
.globl overflow
.globl bounds_check
.globl inval_opcode
.globl copr_not_available
.globl double_fault
.globl copr_seg_overrun
.globl inval_tss
.globl segment_not_present
.globl stack_exception
.globl general_protection
.globl page_fault
.globl copr_error
_start:
	movl	$StackTop,%esp
	sgdt	(gdt_ptr)
	call	cstart
	lgdt	(gdt_ptr)
	lidt	(idt_ptr)
	ljmp	$SELECTOR_KERNEL_CS,$csinit
divide_error:
	pushl	$0xffffffff
	pushl	$0
	jmp		exception
sigle_step_exception:
	pushl	$0xffffffff
	pushl	$1
	jmp		exception
nmi:
	pushl	$0xffffffff
	pushl	$2
	jmp		exception
breakpoint_exception:
	pushl	$0xffffffff
	pushl	$3
	jmp		exception
overflow:
	pushl	$0xffffffff
	pushl	$4
	jmp		exception
bounds_check:
	pushl	$0xffffffff
	pushl	$5
	jmp		exception
inval_opcode:
	pushl	$0xffffffff
	pushl	$6
	jmp		exception
copr_not_available:
	pushl	$0xffffffff
	pushl	$7
	jmp		exception
double_fault:
	pushl	$8
	jmp		exception
copr_seg_overrun:
	pushl	$0xffffffff
	pushl	$9
	jmp		exception
inval_tss:
	pushl	$10
	jmp		exception
segment_not_present:
	pushl	$11
	jmp		exception
stack_exception:
	pushl	$12
	jmp		exception
general_protection:
	pushl	$13
	jmp		exception
page_fault:
	pushl	$14
	jmp		exception
copr_error:
	pushl	$0xffffffff
	pushl	$16
	jmp		exception
exception:
	call	exception_handler
	add		$8,%esp
	hlt
csinit:
	ud2
	hlt
