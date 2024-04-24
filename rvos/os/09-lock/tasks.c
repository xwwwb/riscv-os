#include "os.h"

#define DELAY 1000

void task0(void)
{
    r_mstatus();
    printf("Task 0: Created!\n");
    while (1)
    {
        spin_lock();
        for (int i = 0; i < 5; i++)
        {
            printf("Task 0: Running...\n");

            task_delay(DELAY);
        }
        spin_unlock();
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

void os_main()
{
    task_create(task0);
    task_create(task1);
}