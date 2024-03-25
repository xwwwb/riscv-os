# and指令
# and rd, rs1, rs2
# 逻辑与运算
 
.text
.global _start
_start:
    li x6, 0x10 # 0x00000010
    li x7, 0x11 # 0x00000011
    and x5, x6, x7 # 0x00000010

stop:
    j stop

.end