.section .data
disp_pos:	.long	0
.section .text
.globl gotoxy
.globl out_byte
.globl in_byte
.globl cls
.globl disp_color_str

// void disp_color_str(char* info,u8 color)
disp_color_str:
	push	%ebp
	movl	%esp,%ebp
	push	%esi
	movl	8(%ebp),%esi
	cld
1:	lodsb
	test	%al,%al
	jz		1f
	movb	12(%ebp),%ah
	call	disp_char
	jmp		1b
1:	pop		%esi
	pop		%ebp


// disp_char al
disp_char:
	push	%edi
	movl	(disp_pos),%edi
	cmp		$7,%al
	jz		char_bel
	cmp		$'\b',%al	# 08
	jz		char_bs
	cmp		$'\t',%al	# 09
	jz		char_ht
	cmp		$'\n',%al	# 0a
	jz		char_lf
	cmp		$0xb,%al	# 0b \v
	jz		char_vt
	cmp		$'\f',%al	# 0c
	jz		char_ff
	cmp		$'\r',%al	# 0d
	jz		char_cr
	movw	%ax,%gs:(,%edi,2)
	inc		%edi
	jmp		char_end
char_bel:
	push	%edi
	push	%ecx
	push	%edx

	movw	$1000,%di
	movb	$0xb6,%al
	out		%al,$0x43
	movw	$0x12,%dx
	movw	$0x348C,%ax
	div		%di
	out		%al,$0x42
	movb	%ah,%al
	out		%al,$0x42

	in		$0x61,%al
	movb	%al,%ah
	or		$3,%al
	out		%al,$0x61

	movl	$1000000,%ecx
	loop	.

	in		$0x61,%al
	and		$0xfc,%al
	out		%al,$0x61
	
	pop		%edx
	pop		%ecx
	pop		%edi
	jmp		char_end
char_bs:
	dec		%edi
	jmp		char_end
char_ht:
	andl	$0xfffffff8,%edi
	addl	$8,%edi
	jmp		char_end
char_lf:
	pushl	%ebx
	pushl	%edx
	xor		%edx,%edx
	movl	%edi,%eax
	movl	$80,%ebx
	div		%ebx
	inc		%eax
	mul		%ebx
	movl	%eax,%edi
	popl	%edx
	popl	%ebx
	jmp		char_end
char_vt:
	addl	$80,%edi
	jmp		char_end
char_ff:
	movb	$3,%ah
	movb	$0,%bh
	int		$0x10
	add		%ch,%dh
	dec		%dh
	add		%cl,%dl
	dec		%dl
	movw	$0x600,%ax
	movw	$0x700,%bx
	int		$0x10
	xor		%edi,%edi
	jmp		char_end
char_cr:
	pushl	%ebx
	pushl	%edx
	xor		%edx,%edx
	movl	%edi,%eax
	movl	$80,%ebx
	div		%ebx
	mul		%ebx
	movl	%eax,%edi
	popl	%edx
	popl	%ebx
	jmp		char_end
char_end:
	movl	%edi,(disp_pos)
	pop		%edi
	ret

//	void gotoxy(int x,int y);
gotoxy:
	push	%ebp
	movl	%esp,%ebp
	push	%ebx
	push	%edx
	xor		%edx,%edx
	movl	8(%ebp),%eax
	movl	$80,%ebx
	mul		%ebx
	add		12(%ebp),%eax
	movl	%eax,(disp_pos)
	pop		%edx
	pop		%ebx
	pop		%ebp
	ret

//	void out_byte(u16 port,u8 value);
out_byte:
	movw	4(%esp),%dx
	movb	8(%esp),%al
	out		%al,%dx
	nop
	nop
	ret

//	u8 in_byte(u16 port)
in_byte:
	movw	4(%esp),%dx
	in		%dx,%al
	nop
	nop
	ret

// void cls(u8 x,u8 y,u8 width,u8 height);
cls:
	push	%ebp
	sub		%esp,%ebp
	push	%ebx
	push	%ecx
	push	%edx
	movb	8(%ebp),%cl
	movb	12(%ebp),%ch
	movb	16(%ebp),%dl
	movb	20(%ebp),%dh
	cmp		$0xff,%dl
	jnz		1f
	movb	$80,%dl
1:	cmp		$0xff,%dl
	jnz		1f
	movb	$25,%dh
1:	add		%cl,%dl
	dec		%dl
	add		%ch,%dh
	dec		%dh
	movw	$0x600,%ax
	movw	$0x700,%bx
	int		$0x10
	pop		%edx
	pop		%ecx
	pop		%ebx
	pop		%ebp
	ret
