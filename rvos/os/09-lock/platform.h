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
/** https://github.com/riscv/riscv-plic-spec/blob/master/riscv-plic.adoc#2-memory-map */
/** 优先级寄存器 设置某一中断源的优先级 从PLIC_BASEADDR开始 每次增长4字节 */
#define PLIC_PRORITY(id) (PLIC_BASEADDR + (id) * 4)
/** 使能寄存器 中断源的开关 */
/** 当hart为0的时候 地址就是PLIC_BASEADDR + 0x2000 此处控制着hart0的0-31位中断号 (1 << UART0_IRQ) 即打开相应hart的使能 */
#define PLIC_ENABLE(hart) (PLIC_BASEADDR + 0x2000 + (hart) * 0x80)
/** 阈值寄存器 针对某一个hart的阈值 当中断的优先级小于等于该hart阈值 被舍掉 每跨越一个hart加0x1000 */
#define PLIC_THRESHOLD(hart) (PLIC_BASEADDR + 0x200000 + (hart) * 0x1000)
/** claim寄存器 用于hart读取当前中断号  每跨域一个hart加0x1000 */
#define PLIC_CLAIM(hart) (PLIC_BASEADDR + 0x200004 + (hart) * 0x1000)
/** complete寄存器 用于标记该hart的中断处理完成 */
#define PLIC_COMPLETE(hart) (PLIC_BASEADDR + 0x200004 + (hart) * 0x1000)

/** 这个写在virt的内存映射中 */
#define CLINT_BASE 0x2000000L
/** 这两个寄存器写在了riscv特权指令标准 mtime会持续自增 大于timecmp的时候发生timer 中断
 *  timer的使能就是mie的mtie位
 *  这里的具体地址映射找不到依据
 *  https://github.com/qemu/qemu/blob/62dbe54c24dbf77051bafe1039c31ddc8f37602d/hw/riscv/virt.c#L1487
 *  只能理解为 virt采用了sifive的然后通过宏的大小计算出来的mtimecmp和mtime地址
 */
#define CLINT_MTIMECMP(hartid) (CLINT_BASE + 0x4000 + 8 * (hartid))
#define CLINT_MTIME (CLINT_BASE + 0xBFF8) // 启动后按照一定频率增加

/**
 * 软中断寄存器
 * https://github.com/qemu/qemu/blob/62dbe54c24dbf77051bafe1039c31ddc8f37602d/include/hw/intc/riscv_aclint.h#L80
 * RISCV_ACLINT_DEFAULT_MTIMECMP      = 0x0,
 * RISCV_ACLINT_DEFAULT_MTIME         = 0x7ff8,
 * RISCV_ACLINT_DEFAULT_MTIMER_SIZE   = 0x8000,
 * RISCV_ACLINT_DEFAULT_TIMEBASE_FREQ = 10000000,
 * RISCV_ACLINT_MAX_HARTS             = 4095,
 * RISCV_ACLINT_SWI_SIZE              = 0x4000
 * 所以SWI在地址映射最前面 所以 mtimecmp是0x0+0x4000=0x4000 起步
 * mtime是也是0x7ff8+0x4000=0xBFF8
 * CLINT_BASE 到 CLINT_BASE + 0x4000是SWI
 * The machine-level MSIP bits are written by accesses to memory-mapped control registers,
 * which are used by remote harts to provide machine-mode interprocessor interrupts.
 * 向SWI最低位写入1触发软中断 写入0清除软中断
 */
#define CLINT_SWI(hartid) (CLINT_BASE + 0x0 + 4 * (hartid))
#endif