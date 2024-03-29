# Get all assembly and C sources
SOURCES = $(shell find kernel/ -type f -name '*.asm' -o -name '*.c')

# Get all C headers
C_HEADERS = $(shell find kernel/ -type f -name '*.h')


# Add all assembly and C files to list of OBJ files
OBJ_FILES = $(patsubst %.c,%.co.o,$(patsubst %.asm,%.o,$(SOURCES)))

# Various flags for command line programs

# Linker flags
LD_FLAGS = -z max-page-size=0x1000

# Compiler flags
C_FLAGS = -g -nostartfiles -ffreestanding -Wall -Wextra -fno-exceptions -I ./ -I ./kernel/lib -mno-red-zone -mcmodel=kernel

# Assembler flags
ASM_FLAGS = -f elf64


# Compiler
CC = x86_64-elf-gcc

# Linker
LD = x86_64-elf-ld

# Assembler
ASM = nasm

# Emulator
EMULATOR = qemu-system-x86_64

# GDB
GDB = x86_64-elf-gdb


# Rule to build the entire project


# Rule to build the kernel elf file
kernel.elf: kernel/boot/mboot.o ${OBJ_FILES}
	${LD} ${LD_FLAGS} -S -o $@ -Tlink.ld $^

debug_kernel.elf: kernel/boot/mboot.o ${OBJ_FILES}
	${LD} ${LD_FLAGS} -o kernel.elf -Tlink.ld $^

# Rule to build the grub iso
grub: kernel.elf
	# Move the kernel elf file to the boot folder
	mv kernel.elf image/boot/kernel.elf

	# Create the iso
	grub-mkrescue -o image.iso image/

# Builds grub with debug elf file
debug_grub: debug_kernel.elf
	# Move the kernel elf file to the boot folder
	mv kernel.elf image/boot/kernel.elf

	# Create the iso
	grub-mkrescue -o image.iso image/

# Rule to run the kernel
run: grub
	${EMULATOR} -bios /usr/share/ovmf/OVMF.fd -hda image.iso -serial file:serial.txt -m 8G

# Run kernel in bios mode
bios: grub
	${EMULATOR} -hda image.iso -serial file:serial.txt -m 8G

# Run the kernel in debug
debug: debug_grub
	${EMULATOR} -S -s -no-shutdown -bios /usr/share/ovmf/OVMF.fd -hda image.iso -serial file:serial.txt -d int -no-reboot -m 8G
	${GDB} -x debug/defaultqemu.gdb


# Here are rules for resolving object file translations

# We have two different object file extensions
# so that we can have conflicting filenames
# between assembly and C
# For example: idt.c and idt.asm

# First C object files
%.co.o: %.c ${C_HEADERS}
	${CC} ${C_FLAGS} -c $< -o $@


# Then assembly object files
%.o: %.asm
	${ASM} $< ${ASM_FLAGS} -o $@


# The clean rule removes all generated files
clean:
	# Compiled kernel
	rm -rf kernel.elf image/boot/kernel.elf image.iso

	# All object files
	find . -type f -name '*.o' -delete