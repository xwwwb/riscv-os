# 加载内存地址
# LA RD, Address
# 将内存地址拷贝到RD中
# 这是一个伪指令
# AUIPC RD, Upper-20
# ADDI RD, RD, Lower-12

.text
.global _start

_start:

    la x5, _start
    jr x5
stop:
    j stop

exit:

.end
