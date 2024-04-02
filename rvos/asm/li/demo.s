# li指令 load immediate
# 直接将立即数放入寄存器中
# 格式 
#    LI RD, IMM
# 描述
# 立即数（任何32位的值）被拷贝到寄存器RD中
# LI是一个伪指令 他实际的汇编指令会根据实际情况不同而不同
# 如果立即数在-2048到+2047之间 可以被汇编为
# ADDI RD, x0, IMM
# 如果立即数不在上面的区间内 但是在32位数的区间内 -2147483648到+2147483647将会被汇编为
# 两步指令
# LUI RD, Upper-20
# ADDI RD, RD, Lower-12
# Upper-20代表立即数的高20位 Lower-12代表立即数的低12位
# 由于addi会将立即数进行符号扩展 所以有的时候会用减法替代

.text
.global _start

_start:
    # 加载一个小数
    li x5, 0x80

    addi x5, x0, 0x80

    li x6, 0x12345001
    
    lui x6, 0x12345
    addi x6, x6, 0x001

    li x7, 0x12345FFF

    lui x7, 0x12346
    addi x7, x7, -1

    # 0xffffffff就是-1
    li x7, 0xffffffff

stop: j stop

.end
