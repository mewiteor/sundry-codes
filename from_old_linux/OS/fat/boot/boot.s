.code16
.set	BaseOfStack,	0x7c00
.include "load.h"
.text
	jmp		1f
	nop
.include "fat12hdr.h"
1:
	movw	%cs,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%ss
	movw	$BaseOfStack,%sp

	/* 清屏 */
	movw	$0x600,%ax
	movw	$0x700,%bx
	xor		%cx,%cx
	movw	$0x184f,%dx
	int		$0x10

	/* 显示 "Booting**" */
	xor		%dh,%dh
	call	DispStr

	call ResetFloppy

	/* 找 LOADER.BIN */
	movw	$BaseOfLoader,%ax
	movw	%ax,%es
	movw	$LoaderFileName,%si
	movw	$RootDirSectors,%ax
	movw	$SecNoOfRootDir,%bx
	movw	$OffsetOfLoader,%di
	call	FindFile
	cmp		$0xffff,%ax
	jnz		1f

	movb	$2,%dh
	call	DispStr
	jmp		.

1:
	push	%ax
	movw	$BaseOfLoader,%ax
	movw	%ax,%es
	movw	$OffsetOfLoader,%bx
	pop		%ax
	call	LoadFile

	movb	$1,%dh
	call	DispStr

	jmp		$BaseOfLoader,$OffsetOfLoader


/*=======================================
	字符串表
  =======================================*/
LoaderFileName:		.ascii	"LOADER  BIN"
.set	MessageLength,9
BootMessage:		.ascii	"Booting**"
Message1:			.ascii	"Loaded in"
Message2:			.ascii	"No LOADER"

/*========================================
	DispStr
	输出索引为dh的字符串
  ========================================*/
DispStr:
	movw	$MessageLength,%ax
	mul		%dh
	add		$BootMessage,%ax
	movw	%ax,%bp
	movw	%ds,%ax
	movw	%ax,%es
	movw	$MessageLength,%cx
	movw	$0x1301,%ax
	movw	$7,%bx
	xor		%dl,%dl
	int		$0x10
	ret

.include "SectorOperator.h"

.org	510
.word	0xaa55
