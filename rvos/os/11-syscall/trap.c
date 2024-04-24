#include "types.h"
#include "os.h"
/** entry.S */
extern void trap_vector(void);
/** uart.c */
extern void uart_receive();
/** timer.c */
extern void timer_handler();
/** sched.c */
extern void schedule(void);
/** syscall.c */
extern void do_syscall(struct context *ctx);

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

reg_t trap_handler(reg_t mepc, reg_t mcause, reg_t mtval, struct context *ctx)
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
        { // M模式的软件中断
            reg_t hartid = r_tp();
            *(uint32_t *)CLINT_SWI(hartid) = 0;
            schedule();
            break;
        }
        case 7:
            // timer中断
            timer_handler();
            break;
        case 11:
            // 外部中断
            interrupt_handler();
            break;
        default:
            printf("Unknown interrupt\n");
            break;
        }
    }
    else
    {
        // 异常
        switch (cause_code)
        {
        case 0:
            printf("Instruction address misaligned\n");
            break;
        case 1:
            printf("Instruction access fault\n");
            printf("%d", mtval);
            break;
        case 2:
            printf("Illegal instruction\n");
            printf("%d", mtval);
        case 5:
            printf("Load access fault\n");
            printf("%d", mtval);
            break;
        case 8:
            printf("System call from User mode\n");
            do_syscall(ctx);
            return_pc += 4;
            break;
        default:
            printf("Exception code: %d\n", cause_code);
            break;
        }
        }

    return return_pc;
}