.text
.global _start
.global foo

_start:
    la sp, stack_end
    li a0, 5
    li a1, 10
    call foo

stop:
    j stop



stack_start:
    .rept 12
    .word 0
    .endr
stack_end:


.end
