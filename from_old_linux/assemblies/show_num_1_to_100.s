.section .data
num:
.space 32
.ascii "\n"
index:
.byte 0
.section .text
putnum:
	movq $32,%rcx
	movq $num,%rdi
	movq $10,%r8
1:
	xor %rdx,%rdx
	div %r8
	add $0x30,%dl
	movb %dl,-1(%rdi,%rcx)
	test %rax,%rax
	loopnz 1b
	movq $4,%rax
	movq $1,%rbx
	movq $33,%rdx
	sub %rcx,%rdx
	lea (%rdi,%rcx),%rcx
	int $0x80
	ret

.globl _start
_start:
1:
	movq $index,%rax
	movzbq (%rax),%rax
	call putnum
	movq $index,%rax
	incb (%rax)
	cmpb $100,(%rax)
	jb 1b
	movq $1,%rax
	xor %rbx,%rbx
	int $0x80
