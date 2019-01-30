.section .text
#selection(unsigned long long *rdi, size_t rsi)
.globl selection
selection:
	xor %rbx,%rbx
	lea -1(%rsi),%rdx
1:
	cmp %rdx,%rbx
	jae 1f
	movq %rbx,%r8
	lea 1(%rbx),%rcx
2:
	cmp %rsi,%rcx
	jae 2f
	movq (%rdi,%r8,8),%rax
	cmp %rax,(%rdi,%rcx,8)
	cmovbq %rcx,%r8
	inc %rcx
	jmp 2b
2:
	movq (%rdi,%r8,8),%rax
	xchg (%rdi,%rbx,8),%rax
	movq %rax,(%rdi,%r8,8)
	inc %rbx
	jmp 1b
1:
	ret
