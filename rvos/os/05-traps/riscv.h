#ifndef __RISCV_H__
#define __RISCV_H__
/** 定义一些寄存器操作和riscv架构的宏 */

/** mtvec寄存器的写入 用于写入陷入处理函数地址 */
static inline void w_mtvec(reg_t v)
{
    asm volatile("csrw mtvec, %0" ::"r"(v));
}

/** 用于mcause寄存器的取值运算 */
#define M_CAUSE_INTERRUPT 0x80000000
#define M_CAUSE_CODE_MASK 0x7FFFFFFF

#endif