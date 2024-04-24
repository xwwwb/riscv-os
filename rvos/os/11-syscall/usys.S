#include "syscall.h"

.global gethartid
gethartid:
    li a7, SYS_gethartid
    ecall
    ret