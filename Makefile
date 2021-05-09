# This is a variable containing all of the c sources.
# This will remain empty until the future
CXX_SOURCES = $(wildcard kernel/*.cpp kernel/**/*.cpp kernel/**/**/*.cpp)

# We also do the same for c headers
CXX_HEADERS = $(wildcard kernel/*.h kernel/**/*.h kernel/**/**/*.h)

# And we do the same for assembly files
ASM_SOURCES = $(wildcard kernel/**/*.asm kernel/**/**/*.asm)

# Now lets create another variable containing all of our object files
OBJECTS = ${CXX_SOURCES:.cpp=.o} # One for c
ASMOBJECTS = ${ASM_SOURCES:.asm=.o} # One for ASM

# Now lets create a variable for all of the flags to be passed to out
# c compiler
CXX_FLAGS = -g -ffreestanding -Wall -Wextra -fno-exceptions -I ./

# Here we declare variables containing the command to access
# Our compiler and linker
CXX = x86_64-elf-g++
LD = x86_64-elf-ld
GDB = x86_64-elf-gdb


# A target to build the grub image
grub: kernel.elf
	mv kernel.elf image/boot/kernel.elf
	grub-mkrescue -o image.iso image/

# A target to build the kernel.elf file
kernel.elf: kernel/asm/boot.o kernel/asm/mboot.o ${ASMOBJECTS} ${OBJECTS} 
	${LD} -z max-page-size=0x1000 -o $@ -Tlink.ld $^

# Runs the kernel
run: grub
	qemu-system-x86_64 -bios /usr/share/ovmf/OVMF.fd -hda image.iso -serial file:serial.txt

bios: grub
	qemu-system-x86_64 -hda image.iso -serial file:serial.txt

bochs: grub
	bochs -f bochsrc.txt

debug: grub
	nohup qemu-system-x86_64 -no-shutdown -s -S -bios /usr/share/ovmf/OVMF.fd -hda image.iso -serial file:serial.txt -d int -no-reboot &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file image/boot/kernel.elf"


# And here we define some rules for resolving wildcard object files
# These will go at the bottom of the file

# First for c files
%.o: %.cpp ${CXX_HEADERS}
	${CXX} ${CXX_FLAGS} -c $< -o $@


# Now for assembly files
%.o: %.asm
	nasm $< -f elf64 -o $@

# Clean
clean:
	rm -rf kernel.elf image/boot/kernel.elf image.iso
	find . -type f -name '*.o' -delete