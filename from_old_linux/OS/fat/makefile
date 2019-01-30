all:boot.img boot/LOADER.BIN kernel/KERNEL.BIN
.PHONY:clean distclean umount

boot.img:boot/BOOT.BIN
	dd if=$< of=$@ bs=512 count=1
	dd if=/dev/zero of=$@ skip=1 seek=1 bs=512 count=2879

boot/BOOT.BIN::
	@cd boot && \
	$(MAKE) && \
	echo done

boot/LOADER.BIN::
	@cd boot && \
	$(MAKE) && \
	echo done

kernel/KERNEL.BIN::
	@cd kernel && \
	$(MAKE) && \
	echo done

copy::
	mkdir -p /tmp/floppy
	sudo mount -o loop boot.img /tmp/floppy/ -o fat=12
	sudo cp boot/LOADER.BIN /tmp/floppy
	sudo cp kernel/KERNEL.BIN /tmp/floppy
	sleep 1
	sudo umount /tmp/floppy/
	rm -rf /tmp/floppy/

clean:
	@cd boot && \
	$(MAKE) clean && \
	echo done
	@cd kernel && \
	$(MAKE) clean && \
	echo done

distclean:
	$(MAKE) clean
	@rm -f *.img
