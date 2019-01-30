.section .text
#partition(unsigned long long *rdi,size_t r8,size_t rsi)
partition:
	dec %r8
	movq %rsi,%r9
	movq (%rdi,%rsi,8),%rax
1:
	inc %r8
	cmp %rax,(%rdi,%r8,8)
	jb 1b
2:	
	dec %rsi
	cmp %r8,%rsi
	jbe 1f
	cmp (%rdi,%rsi,8),%rax
	jb 2b
	movq (%rdi,%rsi,8),%rbx
	xchg (%rdi,%r8,8),%rbx
	movq %rbx,(%rdi,%rsi,8)
	jmp 1b
1:
	xchg %rax,(%rdi,%r8,8)
	movq %rax,(%rdi,%r9,8)
	movq %r8,%rax
	ret
#quick_sub(unsigned long long *rdi,size_t r8,size_t rsi)
quick_sub:
	cmp %rsi,%r8
	jb 1f
	ret
1:
	push %rsi
	push %r8
	call partition
	pop %r8
	push %rax
	test %rax,%rax
	jz 1f
	lea -1(%rax),%rsi
	call quick_sub
1:
	pop %r8
	inc %r8
	pop %rsi
	call quick_sub
	ret
.globl quick
#quick(unsigned long long *rdi,size_t rsi)
quick:
	xor %r8,%r8
	dec %rsi
	call quick_sub
	ret
