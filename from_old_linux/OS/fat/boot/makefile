OBJCOPY=objcopy
TRIM_FLAGS=-R .pdr -R .comment -R .note -S -O binary
AFLAGS=-I include/

LDFILE_BOOT = boot.ld
LDFILE_LOADER = loader.ld

LDFLAGS_BOOT=-T $(LDFILE_BOOT)
LDFLAGS_LOADER=-T $(LDFILE_LOADER)

all:BOOT.BIN LOADER.BIN

.PHONY:clean

BOOT.BIN:boot.s include/fat12hdr.h include/SectorOperator.h include/load.h
	$(AS) -o boot.o $< $(AFLAGS)
	$(LD) boot.o -o boot.elf $(LDFLAGS_BOOT)
	$(OBJCOPY) $(TRIM_FLAGS) boot.elf $@

LOADER.BIN:loader.s include/fat12hdr.h include/SectorOperator.h include/load.h include/pm.h include/lib.h
	$(AS) -o loader.o $< $(AFLAGS)
	$(LD) loader.o -o loader.elf $(LDFLAGS_LOADER)
	$(OBJCOPY) $(TRIM_FLAGS) loader.elf $@

clean:
	rm -f *.o *.elf *.BIN
