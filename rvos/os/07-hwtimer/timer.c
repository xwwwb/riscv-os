#include "os.h"

/* interval ~= 1s */
#define TIMER_INTERVAL 10000000

static uint32_t _tick = 0;

/** 设置定时器 */
void timer_load(int interval)
{
    // 拿到hart
    int id = r_tp();
    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init()
{
    // 定时器设置
    timer_load(TIMER_INTERVAL);
    // 打开mstatus 中的mie中断
    w_mstatus(r_mstatus() | MSTATUS_MIE);

    // 打开mie寄存器中的timer 中断
    w_mie(r_mie() | MIE_MTIE);
}

void timer_handler()
{
    _tick++;
    printf("tick: %d\n", _tick);
    timer_load(TIMER_INTERVAL);
}