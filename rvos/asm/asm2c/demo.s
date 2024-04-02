.text
.global _start
.global foo
_start:
    la sp, stack_end
    li a0, 5
    li a1, 6

    call foo

    # riscv 使用a0和a1传输结果
    # 检查a0

stop:
    j stop

stack_start:
    .rept 12
    .word 0
    .endr
stack_end:

.end
