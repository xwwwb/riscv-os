include ../../common.mk

# .DEFAULT_GOAL 是特殊变量
.DEFAULT_GOAL := all 

all:
# 使用编译器编译源文件为一个标准elf文件
	@${CC} ${CFLAGS} ${SRC} -Ttext=0x80000000 -o ${EXEC}.elf
# 使用objcopy去除掉多余的节 因为CFLAGS里面是有-g的
	@${OBJCOPY} -O binary ${EXEC}.elf ${EXEC}.bin

# 按每个字节显示 并显示该字节对应字符 注意这里的是倒叙
.PHONY : hex
hex: all
	@hexdump -C ${EXEC}.bin

# 反汇编
.PHONY : code
code :all
	@${OBJDUMP} -S ${EXEC}.elf | less

# 用QEMU执行编译结果 
# -S 在启动时并不启动CPU，需要在monitor中运行“c”（或“cont”）命令才能继续运行。
# 它可以配合“-gdb”参数一起使用，启动后，让GDB远程连接到QEMU上，然后再继续运行。
# -s 参数是“-gdb tcp::1234”的简写表达方式，即在TCP 1234端口打开一个GDB服务器。
.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${EXEC}.elf -s -S &
	@${GDB} ${EXEC}.elf -q -x ${GDBINIT}