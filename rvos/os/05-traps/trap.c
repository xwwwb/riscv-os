#include "types.h"
#include "os.h"
/** entry.S */
extern void trap_vector(void);

/** 初始化陷入 主要负责写入陷入处理函数到mtvec寄存器 */
void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}

reg_t trap_handler(reg_t mepc, reg_t mcause)
{
    printf("trap!\n");
    while (1)
        ;
    return mepc;
}