#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"
#include "riscv.h"

#include <stddef.h>
#include <stdarg.h>
/* uart 串口输出 */
extern void uart_puts(char *string);

/* printf 字符串格式化 */
extern int printf(const char *s, ...);
extern void panic(char *s);

/* 宏函数 打印指针地址 */
#define print_addr(des, addr) \
    printf(des);              \
    printf(": ");             \
    printf("0x%x\n", addr);

/* page 分页 */
extern void *page_alloc(int page_num);
extern void page_free(void *p);

/* 内存段信息 */
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;

/* 任务管理的上下文 */
struct context
{
    /* 忽略x0了 zero寄存器 */
    reg_t ra;  // return address
    reg_t sp;  // stack point
    reg_t gp;  // global point
    reg_t tp;  // thread point
    reg_t t0;  // temp 0
    reg_t t1;  // temp 1
    reg_t t2;  // temp 2
    reg_t s0;  // save 0
    reg_t s1;  // save 1
    reg_t a0;  // arg 0
    reg_t a1;  // arg 1
    reg_t a2;  // arg 2
    reg_t a3;  // arg 3
    reg_t a4;  // arg 4
    reg_t a5;  // arg 5
    reg_t a6;  // arg 6
    reg_t a7;  // arg 7
    reg_t s2;  // save 2
    reg_t s3;  // save 3
    reg_t s4;  // save 4
    reg_t s5;  // save 5
    reg_t s6;  // save 6
    reg_t s7;  // save 7
    reg_t s8;  // save 8
    reg_t s9;  // save 9
    reg_t s10; // save 10
    reg_t s11; // save 11
    reg_t t3;  // temp 3
    reg_t t4;  // temp 4
    reg_t t5;  // temp 5
    reg_t t6;  // temp 6
};

/** 任务管理的函数 */
void task_yield();
int task_create(void (*task_function_pointer)(void));
void task_delay(volatile int count);

/** 中断控制器 */
int plic_claim(void);
void plic_complete(int irq);

#endif