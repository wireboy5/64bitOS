/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */


ENTRY(start)
SECTIONS
{
  	. = 0x00001000;
	.boot :
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
	
	. += 0xC0000000;
	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
  	kernOffset = 0xC0000000;
	.text ALIGN (4K) :
	{
		*(.text)
    code = .; _code = .; __code = .;
	}
	.rodata ALIGN (4K) :
	{
		*(.rodata)
    rodata = .; _rodata = .; __rodata = .;
	}
	.data ALIGN (4K) :
	{
		*(.data)
    data = .; _bss = .; __bss = .;
	}
	.bss  ALIGN (4K):
	{
		*(COMMON)
		*(.bss)
    bss = .; _bss = .; __bss = .;
	}
	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;

  end = .; _end = .; __end = .;
} 