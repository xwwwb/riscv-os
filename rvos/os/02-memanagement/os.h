#ifndef __OS_H__
#define __OS_H__

#include "types.h"
#include "platform.h"

#include <stddef.h>
#include <stdarg.h>
/* uart */
extern void uart_init(void);
extern void uart_puts(char *string);

/* printf */
extern int printf(const char *s, ...);
extern void panic(char *s);
// 宏函数 打印指针地址
#define print_addr(des, addr) \
    printf(des);              \
    printf(": ");             \
    printf("0x%x\n", addr);

/* page */
extern void page_init();
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
#endif