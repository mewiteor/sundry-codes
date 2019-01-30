cc=gcc
bin=wifi
obj=wifi.o main.o
cflag= -O3
coflag= -pthread -O3

$(bin):$(obj)
	$(cc) -o $(bin) $(obj) $(coflag)
wifi.o:wifi.c
	$(cc) -c -o wifi.o wifi.c $(cflag)
main.o:main.c
	$(cc) -c -o main.o main.c $(cflag)

.PHONY:clean
clean:
	rm -rf $(bin) $(obj)
