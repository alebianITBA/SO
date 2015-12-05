include Makefile.variables


all:  bootloader kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

print-%:
	@echo $* = $($*)

run: clean all
	./run.sh

.PHONY: bootloader image collections kernel userland all clean
