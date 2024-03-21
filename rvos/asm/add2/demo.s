# add指令执行减法

    .text
    .global _start

# 这里的-2是反码的形式存储的
# 0xfffffffe
_start:
    li x6, 1
    li x7, -2
    add x5, x6, x7

stop:
    j stop

.end
