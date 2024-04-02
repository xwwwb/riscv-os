# 取反操作 实际上就是与-1异或
# NOT RD, RS
# XORI RD, RS, -1

.text
.global _start

_start:
    li x6, 0xffff0000

    not x5, x6
    xori x7, x6, -1

stop:
    j stop

.end