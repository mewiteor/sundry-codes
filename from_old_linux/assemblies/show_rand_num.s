.section .data
output:
.space 16,0x30
.byte '\n'
nnn:
.space 8
file:
.space 8
devran:
.asciz "/dev/urandom"
.section .text
putnum:
	movq $16,%rcx
	xor %rdx,%rdx
	movq $output,%rdi
1:
	movb %al,%dl
	and $0xf,%dl
	shr $4,%rax
	add $0x30,%dl
	cmp $0x39,%dl
	jbe 2f
	add $7,%dl
2:
	movb %dl,-1(%rdi,%rcx)
	test %rax,%rax
	loopnz 1b
	movq $4,%rax
	movq $1,%rbx
	movq $output,%rcx
	movq $17,%rdx
	int $0x80
	ret
.globl _start
_start:
	movq $5,%rax
	movq $devran,%rbx
	xor %rcx,%rcx
	int $0x80
	movq $file,%rsi
	movq %rax,(%rsi)
	movq %rax,%rbx
	movq $3,%rax
	movq $nnn,%rcx
	movq $8,%rdx
	int $0x80
	movq $file,%rsi
	movq (%rsi),%rbx
	movq $6,%rax
	int $0x80
	movq $nnn,%rsi
	movq (%rsi),%rax
	call putnum
	movq $1,%rax
	xor %rbx,%rbx
	int $0x80
