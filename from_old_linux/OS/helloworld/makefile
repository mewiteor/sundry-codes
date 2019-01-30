ldfile=boot.ld
objcopy=objcopy

all :boot.img

boot.o:boot.s
	$(AS) boot.s -o boot.o

boot.elf:boot.o
	$(LD) boot.o -o boot.elf -e c -T $(ldfile)

boot.bin:boot.elf
	$(objcopy) -R .pdr -R .comment -R .note -S -O binary boot.elf boot.bin

boot.img :boot.bin
	dd if=boot.bin of=boot.img bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=2879

clean:
	rm -rf boot.o boot.elf boot.bin
