/**
 * 任务调度
 */

#include "os.h"

/** entry.S */
extern void switch_to(struct context *next);

/** 单个任务的栈大小 */
#define TASK_STACK_SIZE 1024
/** 最大任务数量 */
#define TASK_MAX_NUM 16

/** 创建任务的栈 */
/** 因为栈指针指向的地址要求16-byte对齐 */
uint8_t __attribute__((aligned(16))) task_stack[TASK_MAX_NUM][TASK_STACK_SIZE];
/** 创建单个任务上下文 */
struct context task_ctx[TASK_MAX_NUM];

/** _top最大任务数 _current当前任务号 */
static int _top = 0;
static int _current = -1;

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

    // 打开MIE的软中断位
    w_mie(r_mie() | MIE_MSIE);
}

/** 任务切换 和 任务第一次启动 */
void schedule()
{
    if (_top <= 0)
    {
        panic("Numbers of task should be greater than zero!");
        return;
    }

    // 切换到下一个任务
    _current = (_current + 1) % _top;

    // 取出下一个任务
    struct context *next = &task_ctx[_current];
    switch_to(next);
}

/** 任务创建 */
int task_create(void (*task_function_pointer)(void))
{
    if (_top < TASK_MAX_NUM)
    {
        // 创建新任务
        task_ctx[_top].sp = (reg_t)&task_stack[_top][TASK_STACK_SIZE];
        // 这里一定要改
        task_ctx[_top].epc = (reg_t)task_function_pointer;
        _top++;
        return 0;
    }
    else
    {
        return -1;
    }
}

/** 任务切换 */
void task_yield()
{
    reg_t hartid = r_tp();
    // 触发软中断
    *(uint32_t *)CLINT_SWI(hartid) = 1;
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