.code16
.set		BaseOfStack,		0x100
.include	"load.h"
.text
	jmp		1f
.include	"fat12hdr.h"
.include	"pm.h"
LABEL_GDT:			Descriptor		0,0,0
# 0~4G的可执行段:
LABEL_DESC_FLAT_C:	Descriptor		0,0xfffff,(DA_CR | DA_32 | DA_LIMIT_4K)
# 0~4G的可读写段:
LABEL_DESC_FLAT_RW:	Descriptor		0,0xfffff,(DA_DRW | DA_32 | DA_LIMIT_4K)
# 指向显存的段:
LABEL_DESC_VIDEO:	Descriptor		0xb8000,0xffff,(DA_DRW | DA_DPL3)

.set				GdtLen,			. - LABEL_GDT
GdtPtr:				.word			(GdtLen - 1)						#	段界限
					.long			BaseOfLoaderPhyAddr + LABEL_GDT		#	基地址

# 选择子
.set				SelectorFlatC,	LABEL_DESC_FLAT_C	-	LABEL_GDT
.set				SelectorFlatRW,	LABEL_DESC_FLAT_RW	-	LABEL_GDT
.set				SelectorVideo,	LABEL_DESC_VIDEO	-	LABEL_GDT	+	SA_RPL3

.set				BaseOfStack,	0x100

1:
	movw	%cs,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%ss
	movw	$BaseOfStack,%sp

	movb	$0,%dh
	call	DispStrRealMode

#	得到内存数
	movl	$0,%ebx				#	ebx = 后续值,开始时需为0
	movw	$_MemChkBuf,%di		#	es:di指向一个地址范围描述符结构
1:
	movl	$0xe820,%eax
	movl	$20,%ecx
	movl	$0x534d4150,%edx
	int		$0x15
	jc		1f
	add		$20,%di
	incl	(_dwMCRNumber)
	test	%ebx,%ebx
	jnz		1b
	jmp		2f
1:
	movl	$0,(_dwMCRNumber)
2:
#	下面在A盘的根目录寻找KERNEL.BIN
	call	ResetFloppy

	movw	$BaseOfKernelFile,%ax
	movw	%ax,%es
	movw	$KernelFileName,%si
	movw	$RootDirSectors,%ax
	movw	$SecNoOfRootDir,%bx
	movw	$OffsetOfKernelFile,%di
	call	FindFile
	cmp		$0xffff,%ax
	jnz		1f

	movb	$1,%dh
	call	DispStrRealMode
	jmp		.
1:
	push	%ax
	movw	$BaseOfKernelFile,%ax
	movw	%ax,%es
	movw	$OffsetOfKernelFile,%bx
	pop		%ax
	call	LoadFile

	call	KillMotor

	movb	$2,%dh
	call	DispStrRealMode

#	下面准备跳入保护模式
#	加载 GDTR
	lgdt	GdtPtr
#	关中断
	cli
#	打开地址线A20
	inb		$0x92,%al
	orb		$0b00000010,%al
	outb	%al,$0x92

#	准备切换到保护模式
	movl	%cr0,%eax
	orl		$1,%eax
	movl	%eax,%cr0

#	真正进入保护模式
	ljmpl	$SelectorFlatC,$(BaseOfLoaderPhyAddr+LABEL_PM_START)


KernelFileName:		.ascii	"KERNEL  BIN"
.set		MessageLength,	9
LoadingMessage:		.ascii	"Loading  "
Message1:			.ascii	"No Kernel"
Message2:			.ascii	"Ready    "

/*========================================
	DispStrRealMode
	输出索引为dh的字符串
  ========================================*/
DispStrRealMode:
	movw	$MessageLength,%ax
	mul		%dh
	add		$LoadingMessage,%ax
	movw	%ax,%bp
	movw	%ds,%ax
	movw	%ax,%es
	movw	$MessageLength,%cx
	movw	$0x1301,%ax
	movw	$7,%bx
	xor		%dl,%dl
	int		$0x10
	ret

.include	"SectorOperator.h"

LABEL_PM_START:
.code32
	movw	$SelectorVideo,%ax
	movw	%ax,%gs
	movw	$SelectorFlatRW,%ax
	movw	%ax,%ds
	movw	%ax,%es
	movw	%ax,%fs
	movw	%ax,%ss
	movl	$TopOfStack,%esp

	push	$szMemChkTitle
	call	DispStr
	add		$4,%esp

	call	DispMemInfo
	call	SetupPaging

	call	InitKernel
	ljmp	$SelectorFlatC,$KernelEntryPointPhyAddr

.include	"lib.h"

/***************************************
 * 显示内存信息
 ***************************************/
DispMemInfo:
	push	%edi
	push	%esi
	push	%ecx
	movl	$MemChkBuf,%esi
	movl	(dwMCRNumber),%ecx	# for(int i=0;i<*dwMCRNumber;++i)
1:								# {
	movl	$5,%edx				#	for(int j=0;j<5;++j)
	movl	$ARDStruct,%edi
2:	pushl	(%esi)				#	{
	call	DispInt				#		DispInt(MemChkBuf[j*4]);
	cmp		$1,%edx
	jz		3f
	movb	$'\t',%al
	call	DispChar
3:	
	popl	%eax
	stosl						#		ARDStruct[j*4]=MemChkBuf;[j*4];
	add		$4,%esi
	dec		%edx
	test	%edx,%edx
	jnz		2b
	movb	$'\n',%al
	call	DispChar			#		putchar('\n');
	cmpl	$1,(dwType)			#		if(Type==AddressRangeMemory);
	jnz		2f
	movl	(dwBaseAddrLow),%eax
	addl	(dwLengthLow),%eax
	cmpl	(dwMemSize),%eax	#			MemSize=BaseAddrLow+LengthLow;
	jb		2f
	movl	%eax,(dwMemSize)
2:								#	}
	loop	1b					# }
	
	movb	$'\n',%al
	call	DispChar			# putchar('\n');
	push	$szRAMSize
	call	DispStr				# printf("RAM size:");
	add		$4,%esp

	pushl	(dwMemSize)
	call	DispInt				# DispInt(MemSize);
	add		$4,%esp
	pop		%ecx
	pop		%esi
	pop		%edi
	ret

/************************************************
 * 启动分页
 ************************************************/
SetupPaging:
	xor		%edx,%edx
	movl	(dwMemSize),%eax
	movl	$0x400000,%ebx		# 4M,一个页表对应的内存大小
	div		%ebx
	mov		%eax,%ecx			# 此时ecx为页表的个数,也即PDE应该的个数
	test	%edx,%edx
	jz		1f
	inc		%ecx
1:	push	%ecx
	movw	$SelectorFlatRW,%ax
	movw	%ax,%es
	movl	$PageDirBase,%edi
	xor		%eax,%eax
	movl	$(PageTblBase | PG_P | PG_USU | PG_RWW),%eax
	cld
1:	stosl
	add		$4096,%eax
	loop	1b

	pop		%eax
	movl	$1024,%ebx
	mul		%ebx
	movl	%eax,%ecx
	movl	$PageTblBase,%edi
	xor		%eax,%eax
	movl	$(PG_P | PG_USU | PG_RWW),%eax
1:	stosl
	addl	$4096,%eax
	loop	1b

	movl	$PageDirBase,%eax
	movl	%eax,%cr3
	movl	%cr0,%eax
	or		$0x80000000,%eax
	movl	%eax,%cr0
	ret

/************************************************
 * 初始化内核
 ************************************************/
InitKernel:
	push	%esi
	push	%ecx
	xor		%esi,%esi
	xor		%ecx,%ecx
	movw	(BaseOfKernelFilePhyAddr + 0x2C),%cx
	movl	(BaseOfKernelFilePhyAddr + 0x1C),%esi
	addl	$BaseOfKernelFilePhyAddr,%esi
1:	cmp		$0,(%esi)
	jz		2f
	pushl	0x10(%esi)
	movl	4(%esi),%eax
	addl	$BaseOfKernelFilePhyAddr,%eax
	pushl	%eax
	pushl	8(%esi)
	call	MemCpy
	addl	$12,%esp
2:	xor		%eax,%eax
	movw	(BaseOfKernelFilePhyAddr + 0x2A),%ax
	addl	%eax,%esi
	loop	1b
	pop		%ecx
	pop		%esi
	ret

#	符号定义
#	实模式
_szMemChkTitle:			.string	"BaseAddrL\tBaseAddrH\tLengthLow\tLengthHigh\tType\n"
_szRAMSize:				.string	"RAM size:\t"

_dwMCRNumber:			.long	0
_dwDispPos:				.long	(80 * 6)
_dwMemSize:				.long	0
_ARDStruct:
	_dwBaseAddrLow:		.long	0
	_dwBaseAddrHigh:	.long	0
	_dwLengthLow:		.long	0
	_dwLengthHigh:		.long	0
	_dwType:			.long	0
_MemChkBuf:				.space	256
StackSpace:				.space 	1024
#	保护模式
.set	szMemChkTitle,			BaseOfLoaderPhyAddr + _szMemChkTitle
.set	szRAMSize,				BaseOfLoaderPhyAddr + _szRAMSize
.set	dwDispPos,				BaseOfLoaderPhyAddr + _dwDispPos
.set	dwMemSize,				BaseOfLoaderPhyAddr + _dwMemSize
.set	dwMCRNumber,			BaseOfLoaderPhyAddr + _dwMCRNumber
.set	ARDStruct,				BaseOfLoaderPhyAddr + _ARDStruct
.set		dwBaseAddrLow,		BaseOfLoaderPhyAddr + _dwBaseAddrLow
.set		dwBaseAddrHigh,		BaseOfLoaderPhyAddr + _dwBaseAddrHigh
.set		dwLengthLow,		BaseOfLoaderPhyAddr + _dwLengthLow
.set		dwLengthHigh,		BaseOfLoaderPhyAddr + _dwLengthHigh
.set		dwType,				BaseOfLoaderPhyAddr + _dwType
.set	MemChkBuf,				BaseOfLoaderPhyAddr + _MemChkBuf
.set	TopOfStack,				BaseOfLoaderPhyAddr + .	# 栈顶
