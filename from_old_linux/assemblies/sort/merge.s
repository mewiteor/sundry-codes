.section .text
# mergesub(unsigned long long *rdi,size_t r8,size_t r9)
mergesub:
	movq %r8,%rax
	add %r9,%rax
	movq %rax,%r10
	shl $3,%rax
	sub %rax,%rsp
	xor %rbx,%rbx
	movq %r8,%rcx
	xor %rdx,%rdx
	movq %r8,%r11
1:	
	cmp %r11,%rbx
	jae 1f
	cmp %r10,%rcx
	jae 1f
	movq (%rdi,%rbx,8),%rax
	cmp (%rdi,%rcx,8),%rax
	jbe 2f
	movq (%rdi,%rcx,8),%rax
	movq %rax,(%rsp,%rdx,8)
	inc %rcx
	inc %rdx
	jmp 1b
2:
	movq %rax,(%rsp,%rdx,8)
	inc %rbx
	inc %rdx
	jmp 1b
1:
	movq %rdi,%r12
	sub %rbx,%r11
	jbe 1f
	lea (%rdi,%rbx,8),%rsi
	lea (%rsp,%rdx,8),%rdi
	movq %r11,%rcx
	cld
	rep movsq
	jmp 2f
1:
	sub %rcx,%r10
	lea (%rdi,%rcx,8),%rsi
	lea (%rsp,%rdx,8),%rdi
	movq %r10,%rcx
	cld
	rep movsq
2:
	movq %r12,%rdi
	movq %rsp,%rsi
	movq %r8,%rax
	add %r9,%rax
	movq %rax,%rcx
	cld
	rep movsq
	shl $3,%rax
	add %rax,%rsp
	ret

.globl merge
# merge(unsigned long long *rdi,size_t rsi)
merge:
	cmp $2,%rsi
	jb 1f	
	pushq %rdi
	movq %rsi,%r9
	shr %rsi
	sub %rsi,%r9
	movq %rsi,%r8
	pushq %r9
	pushq %r8
	call merge
	movq 0x10(%rsp),%rdi
	movq (%rsp),%rax
	shl $3,%rax
	add %rax,%rdi
	movq 0x8(%rsp),%rsi
	call merge
	popq %r8
	popq %r9
	popq %rdi
	call mergesub
1:
	ret
