cc=g++
exe=wifi
obj=wifi.o hash.o asms.o

$(exe):$(obj)
	$(cc) -o $(exe) $(obj)
wifi.o:wifi.cpp
	$(cc) -g -c -o wifi.o wifi.cpp
hash.o:hash.cpp hash.h
	$(cc) -g -c -o hash.o hash.cpp
asms.o:asms.cpp asms.h
	$(cc) -g -c -o asms.o asms.cpp
.PHONY:clean
clean:
	rm $(exe) $(obj)
