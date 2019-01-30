.section .text
#max_heapify(unsigned long long *rdi,size_t rsi,size_t r9)
max_heapify:
1:
	movq %r9,%r8     #i
	movq %r9,%rbx    #l
	shl %rbx
	inc %rbx
	cmp %rsi,%rbx
	jb 2f
	ret
2:
	movq (%rdi,%r8,8),%rax
	cmp %rax,(%rdi,%rbx,8)
	cmovaq %rbx,%r9
	inc %rbx
	cmp %rsi,%rbx
	jae 2f
	movq (%rdi,%r9,8),%rax
	cmp %rax,(%rdi,%rbx,8)
	cmovaq %rbx,%r9
2:
	cmp %r9,%r8
	jnz 2f
	ret
2:
	movq (%rdi,%r8,8),%rax
	xchg (%rdi,%r9,8),%rax
	movq %rax,(%rdi,%r8,8)
	jmp 1b
.globl heap
#heap(unsigned long long *rdi,size_t rsi)
heap:
	movq %rsi,%rcx
	inc %rcx
	shr %rcx
1:
	lea -1(%rcx),%r9
	call max_heapify
	loop 1b
1:
	dec %rsi
	jnz 2f
	ret
2:
	movq (%rdi),%rax
	xchg (%rdi,%rsi,8),%rax
	movq %rax,(%rdi)
	xor %r9,%r9
	call max_heapify
	jmp 1b
