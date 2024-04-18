#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/*
 * maximum number of CPUs
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * 但是现在文件改动了
 * https://github.com/qemu/qemu/commit/d4452c692488228e6dce193f4f98ac762588d96a
 * https://github.com/qemu/qemu/commit/28d8c281200f20a060c456c81fd1564f3d119fda
 * https://github.com/qemu/qemu/commit/0631aaae31cccf5ae61e8c67c198e064bfaafc66
 * 现在是512核
 */
#define MAXNUM_CPU 8

/** 地址映射中 UART的地址 */
#define UART0 0x10000000L

/** UART在PLIC注册的中断号 */
/** https://github.com/qemu/qemu/blob/62dbe54c24dbf77051bafe1039c31ddc8f37602d/include/hw/riscv/virt.h#L91 */
#define UART0_IRQ 10

/** PLIC 可编程中断控制器的内存映射 */
/** https://github.com/qemu/qemu/blob/62dbe54c24dbf77051bafe1039c31ddc8f37602d/hw/riscv/virt.c#L82 */
#define PLIC_BASEADDR 0xC000000L
/** PLIC 中断寄存器的偏移 */
/** 不同寄存器的偏移量 */
/** https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h */
/** 优先级寄存器 设置某一中断源的优先级 */
#define PLIC_PRORITY(id) (PLIC_BASEADDR + (id) * 4)
/** 使能寄存器 中断源的开关 */
#define PLIC_ENABLE(id) (PLIC_BASEADDR + 0x2000 + (id) * 0x80)
/** 阈值寄存器 针对某一个hart的阈值 当中断的优先级小于等于该hart阈值 被舍掉 */
#define PLIC_THRESHOLD(hart) (PLIC_BASEADDR + 0x200000 + (hart) * 0x1000)
/** claim寄存器 用于hart读取当前中断号 */
#define PLIC_CLAIM(hart) (PLIC_BASEADDR + 0x200004 + (hart) * 0x1000)
/** complete寄存器 用于标记该hart的中断处理完成 */
#define PLIC_COMPLETE(hart) (PLIC_BASEADDR + 0x200004 + (hart) * 0x1000)

#endif