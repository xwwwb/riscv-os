#ifndef __RISCV_H__
#define __RISCV_H__
/** 定义一些寄存器操作和riscv架构的宏 */

/** tp寄存器的读取 在start.S 中把hartid写到tp了 */
static inline reg_t r_tp()
{
    reg_t x;
    asm volatile("mv %0, tp" : "=r"(x));
    return x;
}

/** mtvec寄存器的写入 用于写入陷入处理函数地址 */
static inline void w_mtvec(reg_t v)
{
    asm volatile("csrw mtvec, %0" ::"r"(v));
}

/** 用于mcause寄存器的取值运算
 * mcause最高位是1表示是中断 0表示是异常
 * mcause的低位是异常码或者中断号
 */
#define M_CAUSE_INTERRUPT 0x80000000
#define M_CAUSE_CODE_MASK 0x7FFFFFFF

/** mstatus寄存器的写入 用于控制hart的一些状态 包括中断的打开或者关闭 */
static inline void w_mstatus(reg_t v)
{
    asm volatile("csrw mstatus, %0" ::"r"(v));
}
static inline reg_t r_mstatus()
{
    reg_t v;
    asm volatile("csrr %0, mstatus" : "=r"(v));
    return v;
}

/** mstatus寄存器的位 */
#define MSTATUS_MIE (1 << 3) // machine interrupt enable

/** mie寄存器 控制hart的具体中断的开关 m s u的timer software external */
static inline void w_mie(reg_t v)
{
    asm volatile("csrw mie, %0" ::"r"(v));
}
static inline reg_t r_mie()
{
    reg_t v;
    asm volatile("csrr %0, mie" : "=r"(v));
    return v;
}
/** MIE寄存器的位 */
#define MIE_MEIE (1 << 11) // machine external interrupt enable
#endif