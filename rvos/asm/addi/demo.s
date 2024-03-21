# addi 立即数加
# addi rd, rs1, imm

.text
.global _start

_start:
    li x6,2
    addi x5, x6, 3

stop:
    j stop

    .end
    