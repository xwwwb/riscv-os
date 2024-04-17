#include "os.h"

// 以下函数只运行一次 不必要写在.h中
extern void uart_init(void);
extern void page_init();
extern void task_init();
extern void task_switch_start();
extern void os_main();
extern void trap_init();

void str_copy(char *str, void *dst)
{
    while (*str)
    {
        *(char *)dst++ = *str++;
    }
}

void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RISC-V OS!\n");
    // 实在想试试打印中文 就试一次 省的惹麻烦
    uart_puts("作者: xwwwb\n");

    // 初始化内存分页
    page_init();

    // 初始化陷入
    trap_init();

    // 分配测试
    void *test1 = page_alloc(30);
    void *test2 = page_alloc(50);
    print_addr("memory alloc test1 address", test1);
    print_addr("memory alloc test2 address", test2);
    page_free(test1);
    void *test3 = page_alloc(60);
    print_addr("memory alloc test3 address", test3);
    void *test4 = page_alloc(10);
    print_addr("memory alloc test4 address", test4);
    str_copy("memory read test and string copy test: i love riscv\n", test2);
    printf(test2);
    page_free(test2);
    page_free(test3);
    page_free(test4);

    // 任务调度初始化
    task_init();
    // 创建任务
    os_main();
    // 任务调度开始
    task_switch_start();

    while (1)
    {
    }
}