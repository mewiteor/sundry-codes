# vim: set syntax=asm filetype=asm:
/*======================================
 * FAT12 磁盘头部
 * =====================================*/
BS_OEMName:			.ascii	"Mewiteor"  	# OEM String,必须8字节
BPB_BytePerSec:		.word 	512				# 每扇区字节数
BPB_SecPerCluse:	.byte	1				# 每簇多少扇区
BPB_ResvdSecCnt:	.word	1				# Boot记录占用多少扇区
BPB_NumFATs:		.byte	2				# 共有多少FAT表
BPB_RootEntCnt:		.word	224				# 根目录文件数最大值
BPB_TotSec16:		.word	2880			# 逻辑扇区总数
BPB_Media:			.byte	0xf0			# 媒体描述符
BPB_FATSz16:		.word	9				# 每FAT扇区数
BPB_SecPerTrk:		.word	18				# 每磁道扇区数
BPB_NumHeads:		.word	2				# 磁头数
BPB_HiddSec:		.long	0				# 隐藏扇区数
BPB_TotSec32:		.long	0				# 如果BPB_TotSec16是0，由这个值记录扇区数
BS_DrvNum:			.byte	0				# 中断13的驱动器号
BS_Reserved1:		.byte	0				# 保留
BS_BootSig:			.byte	0x29			# 扩展引导标记（29h）
BS_VolID:			.long	0				# 卷序列号
BS_VolLab:			.ascii	"Mewiter OS"	# 卷标，必须11字节
BS_FILESysType:		.ascii	"FAT12   "		# 文件系统类型，必须8字节

/*========================================================================
 * 基于FAT12头的一些常量定义，如果头信息改变，下面的常量可能也要做相应改变
 * =======================================================================*/
.set				FATSz,			9		# BPB_FATSz16
.set				RootDirSectors,	14		# 根目录占用空间
.set				SecNoOfRootDir,	19		# Root Directory 的第一个扇区号
.set				SecNoOfFAT1,	1		# FAT1的第一个扇区号
.set				DeltaSecNo,		17		# SecNoOfRootDir(19)-(2)
