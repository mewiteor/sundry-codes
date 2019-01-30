.section .data
nums:
.space 80
insertnums:
.space 80
bubblenums:
.space 80
selectionnums:
.space 80
mergenums:
.space 80
quicknums:
.space 80
heapnums:
.space 80

.section .text
.globl _start
_start:
#init
	movq $nums,%rdi
	movq $10,%rsi
	call rands

#insert init
	movq $nums,%rsi
	movq $insertnums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#bubble init
	movq $nums,%rsi
	movq $bubblenums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#selection init
	movq $nums,%rsi
	movq $selectionnums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#merge init
	movq $nums,%rsi
	movq $mergenums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#quick init
	movq $nums,%rsi
	movq $quicknums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#heap init
	movq $nums,%rsi
	movq $heapnums,%rdi
	movq $10,%rcx
	cld
	rep movsq

#insert
	movq $insertnums,%rdi
	movq $10,%rsi
	call insert

#bubble
	movq $bubblenums,%rdi
	movq $10,%rsi
	call bubble

#selection
	movq $selectionnums,%rdi
	movq $10,%rsi
	call selection

#merge
	movq $mergenums,%rdi
	movq $10,%rsi
	call merge

#quick
	movq $quicknums,%rdi
	movq $10,%rsi
	call quick

#heap
	movq $heapnums,%rdi
	movq $10,%rsi
	call heap

#put nums
	movq $nums,%rdi
	movq $10,%rsi
	call putnums

#put insert nums
	movq $insertnums,%rdi
	movq $10,%rsi
	call putnums

#put bubble nums
	movq $bubblenums,%rdi
	movq $10,%rsi
	call putnums

#put selection nums
	movq $selectionnums,%rdi
	movq $10,%rsi
	call putnums

#put merge nums
	movq $mergenums,%rdi
	movq $10,%rsi
	call putnums

#put quick nums
	movq $quicknums,%rdi
	movq $10,%rsi
	call putnums

#put heap nums
	movq $heapnums,%rdi
	movq $10,%rsi
	call putnums

#exit
	movq $1,%rax
	xor %rbx,%rbx
	int $0x80
