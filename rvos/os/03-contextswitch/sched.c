/**
 * 任务调度
 */

#include "os.h"

/** entry.S */
extern void switch_to(struct context *next);

/** 单个任务的栈大小 */
#define TASK_STACK_SIZE 1024

/** 创建任务的栈 */
/** 因为栈指针指向的地址要求16-byte对齐 */
uint8_t __attribute__((aligned(16))) task_stack[TASK_STACK_SIZE];
/** 创建单个任务上下文 */
struct context task_ctx;

/** 写入mscratch寄存器的函数 */
void w_mscratch(reg_t v)
{
    // 没有输出 所以输出空着 有一个输入mscratch 用"r"表示寄存器 用%0表示第一个输入
    asm volatile("csrw mscratch, %0" : : "r"(v));
}

/** 创建一个任务 */
void user_task0(void);

/** 任务调度初始化 */
void task_init()
{
    // 令当前任务指针为0
    w_mscratch(0);
    // 栈底指向最高的地址 看似越界了 实际上指向的是栈的最低地址
    task_ctx.sp = (reg_t)&task_stack[TASK_STACK_SIZE];
    // 这里直接指定了 下一个任务是user_task0
    task_ctx.ra = (reg_t)user_task0;
}

/** 任务切换 */
void schedule()
{
    struct context *next = &task_ctx;
    switch_to(next);
}

/** 延迟 强行延迟 */
void task_delay(volatile int count)
{
    count *= 50000;
    while (count--)
        ;
}

void user_task0(void)
{
    printf("Task 0: Created!");
    while (1)
    {
        printf("Task 0: Running...\n");
        task_delay(1000);
    }
}