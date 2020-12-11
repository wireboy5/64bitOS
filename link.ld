/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */


ENTRY(start)
SECTIONS
{
  . = 0x00001000;

	. += 0xC0000000;
	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
  kernOffset = 0xC0000000;
	.text ALIGN (4K) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
    code = .; _code = .; __code = .;
	}
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000)
	{
		*(.rodata)
    rodata = .; _rodata = .; __rodata = .;
	}
	.data ALIGN (4K) : AT (ADDR (.data) - 0xC0000000)
	{
		*(.data)
    data = .; _bss = .; __bss = .;
	}
	.bss ALIGN (4K) : AT (ADDR (.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
    bss = .; _bss = .; __bss = .;
	}
	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;

  end = .; _end = .; __end = .;
} 