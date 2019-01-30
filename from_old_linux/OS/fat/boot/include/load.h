# vim: set filetype=asm syntax=asm:
.set	BaseOfLoader,				0x9000					# Loader.bin 被加载到的位置 --> 段地址
.set	OffsetOfLoader,				0x0100					# Loader.bin 被加载到的位置 --> 偏移地址

.set	BaseOfLoaderPhyAddr,		(BaseOfLoader * 0x10)	# Loader.bin 被加载到的位置 --> 物理地址

.set	BaseOfKernelFile,			0x8000					# Kernel.bin 被加载到的位置 --> 段地址
.set	OffsetOfKernelFile,			0						# Kernel.bin 被加载到的位置 --> 偏移地址
.set	BaseOfKernelFilePhyAddr,	(BaseOfKernelFile * 0x10)
.set	KernelEntryPointPhyAddr,	0x30400

.set	PageDirBase,				0x200000
.set	PageTblBase,				0x201000
