# vim: set filetype=asm syntax=asm:

/******************************************
 * 显示AL中的数字
 ******************************************/
DispAL:
	push	%edi
	movl	(dwDispPos),%edi
	movb	$0x0f,%ah	# 0b0000: 黑底	0b1111:白字
	push	%ax
	shr		$4,%al
	cmp		$10,%al
	jae		1f
	add		$'0',%al
	jmp		2f
1:	add		$('A'-10),%al
2:	mov		%ax,%gs:(,%edi,2)
	pop		%ax
	and		$0xf,%al
	cmp		$10,%al
	jae		1f
	add		$'0',%al
	jmp		2f
1:	add		$('A'-10),%al
2:	mov		%ax,%gs:2(,%edi,2)
	add		$2,%edi
	movl	%edi,(dwDispPos)
	pop		%edi
	ret

/*******************************************
 * 显示一个整形数
 *******************************************/
DispInt:
	movw	$(0x700 + '0'),%ax	# ah=0b0000:黑底	0b0111:灰字
	push	%edi
	movl	(dwDispPos),%edi
	movw	%ax,%gs:(,%edi,2)
	movb	$'x',%al
	movw	%ax,%gs:2(,%edi,2)
	add		$2,%edi
	movl	%edi,(dwDispPos)
	pop		%edi
	movb	7(%esp),%al
	call	DispAL
	movb	6(%esp),%al
	call	DispAL
	movb	5(%esp),%al
	call	DispAL
	movb	4(%esp),%al
	call	DispAL
	ret

/****************************************
 * 显示一个以0结尾的字符串
 ****************************************/
DispStr:
	push	%ebp
	mov		%esp,%ebp
	push	%esi

	movl	8(%ebp),%esi
	cld
1:	lodsb
	test	%al,%al
	jz		1f
	movb	$0xf,%ah
	call	DispChar
	jmp		1b
1:	pop		%esi
	pop		%ebp
	ret

/***************************************
 * 输出AL字符
 ***************************************/
DispChar:
	push	%edi
	movl	(dwDispPos),%edi
	cmp		$'\n',%al
	jz		1f
	cmp		$'\t',%al
	jz		2f
	movw	%ax,%gs:(,%edi,2)
	inc		%edi
	jmp		3f
2:	and		$0xfffffff8,%edi
	add		$8,%edi
	jmp		3f
1:	push	%ebx
	movw	%di,%ax
	movb	$80,%bl
	div		%bl
	inc		%ax
	mul		%bl
	movw	%ax,%di
	pop		%ebx
3:	movl	%edi,(dwDispPos)
	pop		%edi
	ret


/*****************************************************
 * 内存拷贝,仿memcpy
 * void memcpy(void* es:pDest,void* ds:pSrc,int iSize)
 *****************************************************/
MemCpy:
	push	%ebp
	movl	%esp,%ebp
	push	%edi
	push	%esi
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
	pop		%esi
	pop		%edi
	pop		%ebp
	ret
