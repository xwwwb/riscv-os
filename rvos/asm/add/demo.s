# add指令
# add rd, rs1, rs2
# rs1的值和rs2的值相加放到rd中

.text
.global _start

_start:
    li x5, 7
    li x6, 8
    add x7, x6, x5

stop:
    j stop

.end