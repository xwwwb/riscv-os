#ifndef __RISCV_H__
#define __RISCV_H__

static inline void w_mtvec(reg_t v)
{
    asm volatile("csrw mtvec, %0" ::"r"(v));
}

#endif