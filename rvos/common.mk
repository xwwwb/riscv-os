# 保存工具链的名称
# 交叉编译工具链前缀
CROSS_COMPILE = riscv64-unknown-elf-
# 编译器参数
# 不连接系统标准启动文件和标准库文件
# gcc可能知道很多标准库函数 在编译的时候将他们替换为更有效率的代码或者内联 不进行函数调用
# -fno-builtin就是禁止这些功能
# https://blog.csdn.net/weixin_43844521/article/details/133439268
# -march. The first and most important option is -march . This tells the compiler what code it should produce for the system's processor architecture (or arch); it tells GCC that it should produce code for a certain kind of CPU.
# -mabi https://gcc.gnu.org/onlinedocs/gcc-7.5.0/gcc/RISC-V-Options.html
CFLAGS = -nostdlib -fno-builtin -march=rv32g -mabi=ilp32 -g -Wall

# 模拟器名称
QEMU = qemu-system-riscv32
# 模拟器参数 
# 无图形化 模拟串行端口在控制台上的重定向
# 一个处理器
# 无bios
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump