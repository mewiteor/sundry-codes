.section .text
#insert(unsigned long long *rdi,size_t rsi)
.globl insert
insert:
	movq $1,%rbx
1:
	cmp %rsi,%rbx
	jae 1f
	movq (%rdi,%rbx,8),%rdx
	lea -1(%rbx),%rcx
2:
	cmp $0,%rcx
	jl 2f
	cmp %rdx,(%rdi,%rcx,8)
	jbe 2f
	movq (%rdi,%rcx,8),%rax
	movq %rax,8(%rdi,%rcx,8)
	dec %rcx
	jmp 2b
2:
	movq %rdx,8(%rdi,%rcx,8)
	inc %rbx
	jmp 1b
1:
	ret
