#include "os.h"

#define DELAY 1000

void task0(void)
{
    printf("Task 0: Created!\n");
    while (1)
    {
        printf("Task 0: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

void task1(void)
{
    printf("Task 1: Created!\n");
    while (1)
    {
        printf("Task 1: Running...\n");
        task_delay(DELAY);
        task_yield();
    }
}

void os_main()
{
    task_create(task0);
    task_create(task1);
}