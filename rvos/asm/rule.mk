include ../../common.mk

# .DEFAULT_GOAL 是特殊变量
.DEFAULT_GOAL := all 

all:
# 使用编译器编译源文件为一个标准elf文件
	@${CC} ${CFLAGS} ${SRC} -Ttext=0x80000000 -o ${EXEC}.elf
# 使用objcopy去除掉多余的节 因为CFLAGS里面是有-g的
	@${OBJCOPY} -O binary ${EXEC}.elf ${EXEC}.bin