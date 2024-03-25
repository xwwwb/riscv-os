# 构造一个32位立即数 这个立即数的高20位对应指令中的imm 低12位清零 再和pc相加
# AUIPC rd, imm

.text
.global _start

_start:
    auipc x5, 0x12345

    # 拿到当前的pc值
    auipc x6, 0

stop: j stop

.end

