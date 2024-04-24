#include "os.h"

#define DELAY 1000

struct data
{
    int counter;
    char *str;
};

struct data userdata = {0, "timer test"};

void timer_func(void *arg)
{
    if (NULL == arg)
    {
        return;
    }
    struct data *temp = (struct data *)arg;
    temp->counter++;
    printf("------TIMEOUT: %s\t %d\n", temp->str, temp->counter);
}

// #define LOCK

void task0(void)
{
    printf("Task 0: Created!\n");
    while (1)
    {
#ifdef LOCK
        spin_lock();
#endif
        for (int i = 0; i < 5; i++)
        {
            printf("Task 0: Running...\n");
            task_delay(DELAY);
        }
#ifdef LOCK
        spin_unlock();
#endif
    }
}

void task1(void)
{
    printf("Task 1: Created!\n");
    while (1)
    {
        printf("Task 1: Running...\n");
        task_delay(DELAY);
    }
}

void task_timer_test()
{
    printf("timer test start!\n");
    // 测试定时器
    struct timer *t1 = timer_create(timer_func, &userdata, 5);
    if (t1 == NULL)
    {
        printf("timer create failed!\n");
    }
    struct timer *t2 = timer_create(timer_func, &userdata, 10);
    if (t2 == NULL)
    {
        printf("timer create failed!\n");
    }
    struct timer *t3 = timer_create(timer_func, &userdata, 15);
    if (t3 == NULL)
    {
        printf("timer create failed!\n");
    }
    while (1)
    {
    }
}

void os_main()
{
    task_create(task0);
    task_create(task1);
    task_create(task_timer_test);
}