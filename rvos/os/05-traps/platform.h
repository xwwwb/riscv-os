#ifndef __PLATFORM_H__
#define __PLATFORM_H__


/* 
 * maximum number of CPUs
 * see https://github.com/qemu/qemu/blob/master/include/hw/riscv/virt.h
 * 但是现在文件改动了
 * https://github.com/qemu/qemu/commit/d4452c692488228e6dce193f4f98ac762588d96a
 * https://github.com/qemu/qemu/commit/28d8c281200f20a060c456c81fd1564f3d119fda
 * https://github.com/qemu/qemu/commit/0631aaae31cccf5ae61e8c67c198e064bfaafc66
 * 现在是512核
 */
#define MAXNUM_CPU 8

// 地址映射中 UART的地址
#define UART0 0x10000000L

#endif