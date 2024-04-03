include ../../common.mk

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \

OBJS = ${SRCS_ASM:.S=.o}
OBJS += ${SRCS_C:.c=.o}
# OBJS = start.o kernel.o

.DEFAULT_GOAL := all
all: os.elf

# start.o必须是第一个依赖 $^是所有依赖列表
os.elf: ${OBJS}
	${CC} ${CFLAGS} -Ttext=0x80000000 -o os.elf $^

# $@是目标文件 @< 是第一个依赖名
%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x ../gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf *.o *.bin *.elf
