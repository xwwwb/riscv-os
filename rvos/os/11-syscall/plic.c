#include "os.h"
/** PLIC 中断控制器的初始化 */
void plic_init()
{
    // 拿到当前hart
    int hart = r_tp();

    // RISCV 特权指令的寄存器

    // mstatus 控制当前hart下全局中断的打开或者关闭
    // mstatus下有MIE位 machine interrupt enable
    // 先全局打开machine模式下的中断开关 MIE位在mstatus的第三位
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    // mie 寄存器控制当前hart下不同中断的打开或者关闭
    // 比如u s m 模式下的 timer software external
    // m模式下的external 是 MEIE 位 11位
    w_mie(r_mie() | MIE_MEIE);

    // PLIC 中断控制器的初始化
    // 这里只初始化一个uart的中断 首先看qemu的virt机器的中断号
    // uart注册为10号中断
    // https://github.com/qemu/qemu/blob/62dbe54c24dbf77051bafe1039c31ddc8f37602d/include/hw/riscv/virt.h#L91
    // 1. 需要设置10号中断的优先级
    // 2. 需要设置该hart的阈值
    // 3. 需要设置该hart对中断号使能
    // (reg_t *是标记左值是一个地址 在一个*是解指针写入)
    *(uint32_t *)PLIC_PRORITY(UART0_IRQ) = 1;
    *(uint32_t *)PLIC_THRESHOLD(hart) = 0;
    *(uint32_t *)PLIC_ENABLE(hart) = (1 << UART0_IRQ);
}

/** 拿到中断号 */
int plic_claim(void)
{
    // 中断发生后 claim会保存中断号 从claim的映射地址取值
    int hart = r_tp();
    int irq = *(uint32_t *)PLIC_CLAIM(hart);
    return irq;
}

/** 通知中断已完成 */
void plic_complete(int irq)
{
    int hart = r_tp();
    *(uint32_t *)PLIC_COMPLETE(hart) = irq;
}