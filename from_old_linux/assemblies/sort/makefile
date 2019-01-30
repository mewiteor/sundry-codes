as=as
ld=ld
bin=sort
obj=sort.o rand.o put.o insert.o bubble.o selection.o merge.o quick.o heap.o
flag=-g

$(bin):$(obj)
	$(ld) -o $(bin) $(obj)
sort.o:sort.s
	$(as) -o sort.o sort.s $(flag)
rand.o:rand.s
	$(as) -o rand.o rand.s $(flag)
put.o:put.s
	$(as) -o put.o put.s $(flag)
insert.o:insert.s
	$(as) -o insert.o insert.s $(flag)
bubble.o:bubble.s
	$(as) -o bubble.o bubble.s $(flag)
selection.o:selection.s
	$(as) -o selection.o selection.s $(flag)
merge.o:merge.s
	$(as) -o merge.o merge.s $(flag)
quick.o:quick.s
	$(as) -o quick.o quick.s $(flag)
heap.o:heap.s
	$(as) -o heap.o heap.s $(flag)
clean:
	rm $(bin) $(obj)
