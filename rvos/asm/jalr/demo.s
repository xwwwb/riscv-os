.text
.global _start

_start:
    li x6, 1
    li x7, 2
    jal x5, sum

stop:
    j stop

sum:
    add x6, x6, x7
    jalr x0, 0(x5)

.end
