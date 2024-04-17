#include "os.h"

#define DELAY 1000

void trap_test()
{
    /*
     * Synchronous exception code = 7
     * Store/AMO access fault
     */
    *(int *)0x00000000 = 100;

    /*
     * Synchronous exception code = 5
     * Load access fault
     */
    // int a = *(int *)0x00000000;

    uart_puts("Yeah! I'm return back from trap!\n");
}

void task0(void)
{
    printf("Task 0: Created!\n");
    while (1)
    {
        printf("Task 0: Running...\n");
        // trap测试
        trap_test();
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