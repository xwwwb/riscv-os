#include "os.h"
extern void schedule(void);
/* interval ~= 1s */
#define TIMER_INTERVAL 10000000

static uint32_t _tick = 0;

/** 软件定时器列表 */
#define MAX_TIMER 10
static struct timer timer_list[MAX_TIMER];

/** 设置定时器 */
void timer_load(int interval)
{
    // 拿到hart
    int id = r_tp();
    *(uint64_t *)CLINT_MTIMECMP(id) = *(uint64_t *)CLINT_MTIME + interval;
}

void timer_init()
{
    // 初始化定时器
    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++)
    {
        t->func = NULL;
        t->arg = NULL;
        t++;
    }

    // 定时器设置
    timer_load(TIMER_INTERVAL);

    // 打开mie寄存器中的timer 中断
    w_mie(r_mie() | MIE_MTIE);
}

struct timer *timer_create(void (*func)(void *arg), void *arg, uint32_t expire)
{
    // 判空
    if (NULL == func || 0 == expire)
    {
        return NULL;
    }

    spin_lock();

    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++)
    {
        if (NULL == t->func)
        {
            break;
        }
        t++;
    }

    if (NULL != t->func)
    {
        // 用完了
        spin_unlock();
        return NULL;
    }

    // 分配
    t->func = func;
    t->arg = arg;
    t->expire = _tick + expire;

    spin_unlock();
    return t;
}

// 清除定时器
void timer_delete(struct timer *timer)
{
    spin_lock();

    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++)
    {
        if (t == timer)
        {
            t->func = NULL;
            t->arg = NULL;
            break;
        }
        t++;
    }
    spin_unlock();
}

// 在每次定时器触发的时候都运行该函数检查有没有需要被触发的定时器
static inline void timer_check()
{
    struct timer *t = &(timer_list[0]);
    for (int i = 0; i < MAX_TIMER; i++)
    {
        if (NULL != t->func)
        {
            if (_tick >= t->expire)
            {
                // 执行回调函数
                t->func(t->arg);
                // 执行完后清空
                t->func = NULL;
                t->arg = NULL;
                break;
            }
        }
        t++;
    }
}

void timer_handler()
{
    _tick++;

    timer_check();
    timer_load(TIMER_INTERVAL);

    schedule();
}