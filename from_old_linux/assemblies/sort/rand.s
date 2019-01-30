.section .data
nnn:
.space 8
file:
.space 8
devran:
.asciz "/dev/urandom"
tm:
.long 1,0
.section .text
rand:
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
	cmp $8,%rax
	jz 1f
	xor %rbx,%rbx
	ret
1:
	movq $file,%rsi
	movq (%rsi),%rbx
	movq $6,%rax
	int $0x80
	movq $nnn,%rsi
	movq (%rsi),%rax
	movq $1,%rbx
	ret
.globl rands
rands:
# rands(unsigned long long *rdi,size_t rsi)
	pushq %rdi
	movq %rsi,%rcx
1:
	pushq %rcx
	call rand
	popq %rcx
	test %rbx,%rbx
	jnz 2f
	pushq %rcx
	movq $162,%rax
	movq $tm,%rbx
	xor %rcx,%rcx
	int $0x80
	popq %rcx
	jmp 1b
2:
	movq (%rsp),%rdi
	movq %rax,-8(%rdi,%rcx,8)
	loop 1b
	popq %rdi
	ret
