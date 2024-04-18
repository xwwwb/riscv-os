#include "types.h"
#include "os.h"
/** entry.S */
extern void trap_vector(void);
extern void uart_receive();

/** 初始化陷入 主要负责写入陷入处理函数到mtvec寄存器 */
void trap_init()
{
    w_mtvec((reg_t)trap_vector);
}
void interrupt_handler()
{
    // 拿到中断号
    int irq = plic_claim();
    if (irq == UART0_IRQ)
    {
        // 处理uart中断
        printf("Uart interrupt!\n");
        uart_receive();
    }
    else
    {
        printf("Unhandler interrupt irq = %d", irq);
    }
    if (irq)
    {
        plic_complete(irq);
    }
}

reg_t trap_handler(reg_t mepc, reg_t mcause)
{
    reg_t return_pc = mepc;
    // 除了最高位 都拿
    reg_t cause_code = mcause & M_CAUSE_CODE_MASK;
    if (mcause & M_CAUSE_INTERRUPT)
    {
        // 如果最高位是1 中断
        // 参考手册的 mcause
        switch (cause_code)
        {
        case 3:
            printf("Machine software interrupt\n");
            break;
        case 7:
            printf("Machine timer interrupt\n");
            break;
        case 11:
            interrupt_handler();
            break;
        default:
            printf("Unknown interrupt\n");
            break;
        }
    }
    else
    {
        // 最高位是0 异常
        printf("Exception code: %d\n", cause_code);
        // panic("Unhandled exception");
        return return_pc + 4;
    }

    return return_pc;
}