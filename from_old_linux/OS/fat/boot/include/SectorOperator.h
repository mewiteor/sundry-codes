# vim: set filetype=asm syntax=asm:

	/* 重置软盘 */
ResetFloppy:
	xor		%ah,%ah
	xor		%dl,%dl
	int		$0x13
	ret

/*========================================
	ReadSector
	从ax读cl个扇区到es:bx
  ========================================*/
ReadSector:
	push	%cx
	push	%bx
	mov		(BPB_SecPerTrk),%bl
	div		%bl
	inc		%ah
	movb	%ah,%cl
	movb	%al,%dh
	shr		%al
	movb	%al,%ch
	and		$1,%dh
	pop		%bx
	mov		(BS_DrvNum),%dl
1:
	movb		$2,%ah
	movb		(%esp),%al
	int		$0x13
	jc		1b
	pop		%cx
	ret

/*=========================================
	GetFATEntry
	在FAT中查找ax索引的扇区，结果保存在ax中
  =========================================*/
GetFATEntry:
	push	%bx
	movw	$3,%bx
	mul		%bx
	clc
	rcr		$1,%ax
	setc	%cl
	shl		$2,%cl
	push	%cx
	xor		%dx,%dx
	movw	(BPB_BytePerSec),%bx
	div		%bx
	push	%dx
	xor		%bx,%bx
	add		$SecNoOfFAT1,%ax
	movb	$2,%cl
	call	ReadSector
	pop		%dx
	add		%dx,%bx
	movw	%es:(%bx),%ax
	pop		%cx
	shr		%cl,%ax
	and		$0x0fff,%ax
	pop		%bx
	ret

/*=====================================
 * FindFile 查找文件
 *  si:文件名，
 *	ax:wRootDirSizeForLoop
 *	bx:SecNoOfRootDir
 *	es:di缓存
 *  返回:
 *			ax:簇号
 *			ebx:文件大小
 *=====================================*/

FindFile:
	push	%bp
	movw	%sp,%bp
	/*       sReadSector+wSectorNo+wRootDirSizeForLoop+wFileName  */
.set		sReadSector,			-2-2-2-2
.set		wSectorNo,				-2-2-2
.set		wRootDirSizeForLoop,	-2-2
.set		wFileName,				-2
	sub		$8,%sp
	movw	%di,sReadSector(%bp)
	movw	%bx,wSectorNo(%bp)
	movw	%ax,wRootDirSizeForLoop(%bp)
	movw	%si,wFileName(%bp)
1:
	cmpw	$0,wRootDirSizeForLoop(%bp)
	jz		1f
	decw	wRootDirSizeForLoop(%bp)
	movw	sReadSector(%bp),%bx
	movw	wSectorNo(%bp),%ax
	movb	$1,%cl
	call	ReadSector
	movw	sReadSector(%bp),%di
	cld
	movw	$0x10,%dx
2:
	cmp		$0,%dx
	jz		2f
	dec		%dx
	movw	$11,%cx
	movw	wFileName(%bp),%si
	repz	cmpsb
	test	%cx,%cx
	jz		3f
	and		$0xffe0,%di
	add		$0x20,%di
	jmp		2b
2:
	addw	$1,wSectorNo(%bp)
	jmp		1b
1:
	movw	$0xffff,%ax
	jmp		2f
3:
	and		$0xffe0,%di
	add		$0x1a,%di
	movw	%es:(%di),%ax
	movl	%es:2(%di),%ebx
2:
	add		$8,%sp
	pop		%bp
	ret

/*==========================================
 * LoadFile
 * es:bx	载入位置
 * ax		起始簇号
 *==========================================*/
LoadFile:
	push	%ax
	push	%ax
	push	%bx
	movb	$0xe,%ah
	movb	$'.',%al
	movb	$0xf,%bl
	int		$0x10
	pop		%bx
	pop		%ax
	add		$RootDirSectors,%ax
	add		$DeltaSecNo,%ax
	movb	$1,%cl
	call	ReadSector
	pop		%ax
	push	%es
	movw	%es,%cx
	sub		$0x100,%cx
	movw	%cx,%es
	call	GetFATEntry
	pop		%es
	add		(BPB_BytePerSec),%bx
	cmp		$0xfff,%ax
	jnz		LoadFile
	ret

/* 关闭软驱马达 */
KillMotor:
	push	%dx
	mov		$0x3f2,%dx
	xor		%al,%al
	out		%al,%dx
	pop		%dx
	ret
