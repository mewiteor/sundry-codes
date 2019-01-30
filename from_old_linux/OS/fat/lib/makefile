all:kliba.o string.o

.PHONY:clean

kliba.o:kliba.s
	$(AS) --32 -o $@ $<

string.o:string.s
	$(AS) --32 -o $@ $<

clean:
	rm -f *.o
