/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */


ENTRY(_start)
SECTIONS
{
  	. = 1M;
	.multiboot_header ALIGN (4K) : AT(ADDR(.multiboot_header))
    {
        /* ensure that the multiboot header is at the beginning */
        *(.multiboot_header)
    }
	kernOffset = 0xFFFFFFFF80000000;
	. += kernOffset;
	/* Add a symbol that indicates the start address of the kernel. */
	_kernel_start = .;
  	/*kernOffset = 0xC0000000;*/
	
	
	

	.text ALIGN (4K) : AT (ADDR (.text) - kernOffset)
	{
		*(.text)
    	code = .; _code = .; __code = .;
	}
	
	.rodata ALIGN (4K) : AT (ADDR (.rodata) - kernOffset)
	{
		*(.rodata)
    	rodata = .; _rodata = .; __rodata = .;
	}
	
	.data ALIGN (4K) : AT (ADDR (.data) - kernOffset)
	{
		*(.data)
    	data = .; _data = .; __data = .;
	}

	
	
	.bss  ALIGN (4K): AT (ADDR (.bss) - kernOffset)
	{
		*(COMMON)
		*(.bss)
    	bss = .; _bss = .; __bss = .;
	}

	/* Add a symbol that indicates the end address of the kernel. */
	_kernel_end = .;
	_kernel_end_phys = . - kernOffset;

  	end = .; _end = .; __end = .;

	
} 
