#include "platform.h"
    # 每一个hart的栈都是1024 bytes
    .equ  STACK_SIZE, 1024

    .global _start
    .text

_start:
    # csrr 伪指令 读CSR寄存器 https://github.com/qemu/qemu/blob/master/hw/core/loader.c#L451
    # mhartid CSR寄存器 特权指令PDF P19
    csrr    t0, mhartid
    # 保存处理器的 hartid 后面会用 查询得tp 是x4
    mv      tp, t0
    # branch not equal zero 不为0跳转
    bnez    t0, park

    # 设置BSS段的所有字节为0
	la	a0, _bss_start
	la	a1, _bss_end
    # 按照无符号的形式比较RS1和RS2 如果RS1 >= RS2 跳转到新路径
    # 2f是说 距离当前语句后面的2标签 forward
	bgeu	a0, a1, 2f
1:
	sw	zero, (a0)
	addi	a0, a0, 4
    # 按照无符号的形式比较RS1和RS2 如果RS1 < RS2 跳转到新路径
    # 1b是说 距离当前语句前面的1标签 backward
	bltu	a0, a1, 1b
2:

    # 设置栈
    slli    t0, t0, 10  # 左移hart id 10位 也就是乘1024
    # stacks是静态分配的内存空间 sp指向第一个栈区的栈顶
    la      sp, stacks + STACK_SIZE
    # t0是当前hart id 让sp指向当前hert的栈
    # 但是实际上只有一个hert在工作
    add     sp, sp, t0


    # 初始化mstatus的值 
    # 令MPP位为11 MPIE位为1
    # MPIE作用是 当trap发生时 将先前的MIE值保存到MPIE中
    # 当执行mret时 会将MPIE的值赋给MIE 
    # MPP位作用是 当trap发生时的模式保留在MPP中 然后把权限改为M 因为所有的trap都是在M模式下处理的 暂不考虑委托
    # 执行mret时 也会将权限模式改为MPP的值
    # 如果不做这一步 在第一次调度的时候 直接执行switch_to 会调用mret
    # 会将权限模式改为U 这样在读取mstatus就会报错 执行spin_lock的时候也会报错非法指令
    li	t0, 1 << 7
	csrr	a1, mstatus
	or	t0, t0, a1
	csrw	mstatus, t0

    j       start_kernel



park:
    # wait for interrupt 特权指令PDF P41
    # 执行到wfi后会进入空闲状态 这种状态可以被成为休眠 直到处理器收到了中断
    wfi
    j       park

# 分配栈区
# RISC-V调用约定规范 栈指针sp必须总是16-byte 对齐
.balign 16
stacks:
    # https://sourceware.org/binutils/docs/as/Skip.html
    # MAXNUM_CPU 来自头文件
    .skip   STACK_SIZE * MAXNUM_CPU

    .end
