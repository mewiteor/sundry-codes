.section .text
// void memcpy(void*,void*,unsigned int);
.globl	memcpy
memcpy:
	push	%ebp
	movl	%esp,%ebp
	push	%esi
	push	%edi
	push	%ecx
	movl	8(%ebp),%edi
	movl	12(%ebp),%esi
	movl	16(%ebp),%ecx
	cld
	test	$1,%ecx
	jz		1f
	movsb
1:	test	$2,%ecx
	jz		1f
	movsw
1:	shr		$2,%ecx
	rep		movsl
	pop		%ecx
	pop		%edi
	pop		%esi
	pop		%ebp
	ret
