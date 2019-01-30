CC=gcc
AFLAGS=-I include/
CFLAGS=-I ../include/

all:KERNEL.BIN

.PHONY:clean

KERNEL.BIN:kernel.o start.o global.o mlibs.o protect.o ../lib/kliba.o ../lib/string.o
	$(LD) -s -Ttext 0x30400 -melf_i386 -o $@ $^

kernel.o:kernel.s
	$(AS) --32 -o $@ $<

start.o:start.c ../include/mlibs.h ../include/global.h ../include/protect.h
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

global.o:global.c ../include/type.h ../include/const.h ../include/mlibs.h ../include/protect.h ../include/global.h
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

mlibs.o:mlibs.c ../include/mlibs.h
	$(CC) -fno-stack-protector -m32 -c -o $@ $< $(CFLAGS)

protect.o:protect.c ../include/const.h ../include/type.h ../include/mlibs.h
	$(CC) -m32 -c -o $@ $< $(CFLAGS)

../lib/kliba.o::
	@cd ../lib && \
	$(MAKE) && \
	echo done

../lib/string.o::
	@cd ../lib && \
	$(MAKE) && \
	echo done

clean:
	@cd ../lib && \
	$(MAKE) clean && \
	echo done
	rm -f *.o *.BIN
