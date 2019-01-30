.section .text
#bubble(unsigned long long *rdi,size_t rsi)
.globl bubble
bubble:
# rbx:i,rcx:j
	xor %rbx,%rbx
	lea -1(%rsi),%rdx
1:
	cmp %rdx,%rbx
	jae 1f
	movq %rdx,%rcx
2:
	cmp %rbx,%rcx
	jbe 2f
	movq (%rdi,%rcx,8),%rax
	cmp (%rdi,%rbx,8),%rax
	jae 3f
	xchg (%rdi,%rbx,8),%rax
	movq %rax,(%rdi,%rcx,8)
3:
	loop 2b
2:
	inc %rbx
	jmp 1b
1:
	ret
