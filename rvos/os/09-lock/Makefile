include ../../common.mk

SRCS_ASM = \
	start.S \
	mem.S \
	entry.S \

SRCS_C = \
	kernel.c \
	uart.c \
	printf.c \
	page.c \
	sched.c \
	tasks.c \
	trap.c \
	plic.c \
	timer.c \
	lock.c \


OBJS = ${SRCS_ASM:.S=.o}
OBJS += ${SRCS_C:.c=.o}

.DEFAULT_GOAL := all
all: os.elf

# start.o必须是第一个依赖 $^是所有依赖列表
os.elf: ${OBJS}
	@${CC} ${CFLAGS} -T link_script.ld -o os.elf $^
	@echo -e "\x1b[32;1mCompile done! \x1b[0m"

# $@是目标文件 @< 是第一个依赖名
%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo -e "\x1b[32;1mPress Ctrl-A and then X to exit QEMU\x1b[0m"
	@echo -e "\x1b[32;1m------------------------------------\x1b[0m"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : debugg
debugg: all
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@gdbgui --gdb-cmd="${GDB} os.elf -q -x ../gdbinit"

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
