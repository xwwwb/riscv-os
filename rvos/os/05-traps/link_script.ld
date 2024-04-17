/*
 * 链接脚本
 * 用于指定段的地址
 */

/**
 * 参考资料 https://sourceware.org/binutils/docs/ld/Miscellaneous-Commands.html
 * OUTPUT_ARCH command specifies a particular output machine architecture.
 * "riscv" is the name of the architecture for both 64-bit and 32-bit
 * RISC-V target. We will further refine this by using -march
 * and -mabi when calling gcc.
 */
OUTPUT_ARCH("riscv")

/**
 * 参考资料 https://sourceware.org/binutils/docs/ld/Entry-Point.html
 * 程序被执行的第一条语句 这里的参数是一个符号 _start被定义在start.S文件中
 */
ENTRY(_start)

/**
 * 参考资料 https://sourceware.org/binutils/docs/ld/MEMORY.html
 * The MEMORY command describes the location and size of blocks of memory in
 * the target.
 * The syntax for MEMORY is:
 * MEMORY
 * {
 *     name [(attr)] : ORIGIN = origin, LENGTH = len
 *     ......
 * }
 * Each line defines a memory region.
 * Each memory region must have a distinct name within the MEMORY command. Here
 * we only define one region named as "ram".
 * The "attr" string is an optional list of attributes that specify whether to
 * use a particular memory region for an input section which is not explicitly
 * mapped in the linker script. Here we assign 'w' (writeable), 'x' (executable),
 * and 'a' (allocatable). We use '!' to invert 'r' (read-only) and
 * 'i' (initialized).
 * The "ORIGIN" is used to set the start address of the memory region. Here we
 * place it right at the beginning of 0x8000_0000 because this is where the
 * QEMU-virt machine will start executing.
 * Finally LENGTH = 128M tells the linker that we have 128 megabyte of RAM.
 * The linker will double check this to make sure everything can fit.
 */
MEMORY{
	ram(wxa!ri) : ORIGIN = 0x80000000, LENGTH = 128M
}

/*
 * 用于指定每个段的地址
 * ('.' = current memory location)
 */

SECTIONS
{
	.text : {
		PROVIDE(_text_start = .);
		*(.text .text.*)
		PROVIDE(_text_end = .);
	} > ram

	.rodata :
	{
		PROVIDE(_rodata_start = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	}
	> ram

	.data :
	{
		. = ALIGN(4096);
		PROVIDE(_data_start = .);
		*(.sdata .sdata.*) 
		*(.data .data.*)
		PROVIDE(_data_end = .);
	}
	> ram

	.bss :
	{
		PROVIDE(_bss_start = .);
		*(.sbss .sbss.*) 
		*(.bss .bss.*) 
		*(COMMON)
		PROVIDE(_bss_end = .);
	}
	> ram

	PROVIDE(_memory_start = ORIGIN(ram));
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

	PROVIDE(_heap_start = _bss_end);
	PROVIDE(_heap_size = _memory_end - _heap_start);
}
