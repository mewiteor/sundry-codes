.section .data
pn:
.space 16,0x30
.byte ' '
nn:
.byte '\n'
.section .text
.globl putnums
putnums:
# putnums(unsigned long long *rdi,size_t rsi)
	pushq $0
	pushq %rsi
	pushq %rdi
1:
	movq 16(%rsp),%rcx
	cmp 8(%rsp),%rcx
	jae 1f
	movq (%rsp),%rsi
	movq (%rsi,%rcx,8),%rax
	
	movq $16,%rcx
	movq $pn,%rdi
2:	
	movb %al,%dl
	and $0xf,%dl
	shr $4,%rax
	add $0x30,%dl
	cmp $0x39,%dl
	jbe 3f
	add $7,%dl
3:
	movb %dl,-1(%rdi,%rcx)
	test %rax,%rax
	loopnz 2b
	test %rcx,%rcx
	jz 5f
4:
	movb $0x30,-1(%rdi,%rcx)
	loop 4b
5:	
	movq $4,%rax
	movq $1,%rbx
	movq $pn,%rcx
	movq $17,%rdx
	int $0x80
	movq 16(%rsp),%rax
	inc %rax
	movq %rax,16(%rsp)
	jmp 1b
1:	
	movq $4,%rax
	movq $1,%rbx
	movq $nn,%rcx
	movq $1,%rdx
	int $0x80
	pop %rdi
	pop %rsi
	pop %rax
	ret

