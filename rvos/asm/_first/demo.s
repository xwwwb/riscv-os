# 第一个RISC-V汇编程序

.macro  do_nothing # 宏
    nop
    nop
.endm

    .text              # 伪操作
    .global _start     # 伪操作

_start: # 标签
    li x6, 0x12345FFF
    li x6, 5 # 伪指令
    li x7, 8 # 伪指令
    add x5, x6, x7 # 指令
    do_nothing # 调用宏

stop:   j stop # 单行

    .end # 文件结尾
    