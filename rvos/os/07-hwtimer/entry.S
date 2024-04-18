# 用于定义协作式任务切换的代码
# 定义一个宏 传入的参数是一个指针 将寄存器的值保存到指针指向的内存中
# 类似于struct context *base = &ctx_task;
# base->ra = ra;
# tp和gp寄存器一般会有特殊用途 在本系统中 tp用于保存当前的hartid
#  write `\p' or `\p1' to evaluate the arguments. 这样就是为什么要用\base 
# https://ftp.gnu.org/old-gnu/Manuals/gas-2.9.1/html_node/as_107.html#SEC109
.macro reg_save base
    sw ra, 0(\base)
    sw sp, 4(\base)
    sw t0, 16(\base)
    sw t1, 20(\base)
	sw t2, 24(\base)
	sw s0, 28(\base)
	sw s1, 32(\base)
	sw a0, 36(\base)
	sw a1, 40(\base)
	sw a2, 44(\base)
	sw a3, 48(\base)
	sw a4, 52(\base)
	sw a5, 56(\base)
	sw a6, 60(\base)
	sw a7, 64(\base)
	sw s2, 68(\base)
	sw s3, 72(\base)
	sw s4, 76(\base)
	sw s5, 80(\base)
	sw s6, 84(\base)
	sw s7, 88(\base)
	sw s8, 92(\base)
	sw s9, 96(\base)
	sw s10, 100(\base)
	sw s11, 104(\base)
	sw t3, 108(\base)
	sw t4, 112(\base)
	sw t5, 116(\base)
    # 空出t6寄存器 因为要用t6参与一些运算 等会会单独保存t6
.endm

.macro reg_restore base
    lw ra, 0(\base)
    lw sp, 4(\base)
    lw t0, 16(\base)
    lw t1, 20(\base)
    lw t2, 24(\base)
    lw s0, 28(\base)
    lw s1, 32(\base)
    lw a0, 36(\base)
    lw a1, 40(\base)
    lw a2, 44(\base)
    lw a3, 48(\base)
    lw a4, 52(\base)
    lw a5, 56(\base)
    lw a6, 60(\base)
    lw a7, 64(\base)
    lw s2, 68(\base)
    lw s3, 72(\base)
    lw s4, 76(\base)
    lw s5, 80(\base)
    lw s6, 84(\base)
    lw s7, 88(\base)
    lw s8, 92(\base)
    lw s9, 96(\base)
    lw s10, 100(\base)
    lw s11, 104(\base)
    lw t3, 108(\base)
    lw t4, 112(\base)
    lw t5, 116(\base)
    lw t6, 120(\base) # 这里为什么
.endm

.text
# https://stackoverflow.com/questions/50568399/what-is-the-difference-between-global-and-globl
# .global 和 .globl 是一样的
.globl trap_vector
.balign 4
trap_vector:
    # 交换mscratch和t6的值
    csrrw   t6, mscratch, t6

    # 如果这里的t6是0的话 这里会运行出错 
    # 所以如果没有创建任务的话 就运行trap_vector t6就会为0
    # 将当前任务的上下文保存到t6指向的内存中
    reg_save    t6
    # 还要额外保存一下t6
    mv  t5, t6
    csrr    t6, mscratch
    sw  t6, 120(t5)

    # 存储上下文指针到mscratch 最开始交换了 这里换回来
    csrw    mscratch, t5

    # 接下来需要调用C语言编写的陷入处理函数
    csrr    a0, mepc
    csrr    a1, mcause
    call    trap_handler

    # C语言函数会将返回值放到a0里 也就是return address
    csrw    mepc, a0

    # 当调用完陷入处理函数后 要恢复上下文
    csrr    t6, mscratch
    reg_restore t6

    # 从陷入中恢复用mret
    mret


.globl switch_to
.balign 4
# void switch_to(struct context *next)
switch_to:
    # 交换mscratch和t6
    # mscratch Typically, it is used to hold a pointer to a machine-mode hart-local context space and swapped with a user register upon entry to an M-mode trap handler.
    # CSRRW 先读出 CSR 中的值，将其按 XLEN 位的宽度进行 “零扩展(zero-extend)” 后写入 RD;然后将 RS1 中的值写入 CSR。
    # csrrw rd, csr, rs1
    # mscratch保存的是当前任务的上下文 执行之后 t6 保存的是当前任务的上下文 mscratch保存的是下一个任务的上下文
    csrrw   t6, mscratch, t6
    # 如果是第一次调用switch_to 那么mscratch中的值是0 t6也就是0 直接省去保存当前任务的上下文这一步
    beqz    t6, 1f

    # t6是当前任务的上下文 然后reg_save将当前任务的寄存器保存到t6指向的内存中
    reg_save    t6

    # 因为t5 已经被保存了 所以用t5保存t6 也就是当前任务的上下文
    mv t5, t6
    # 从mscratch中读取t6在执行switch_to之前的值 为了单独保存一下t6
    # 伪指令 将csrrw的rd设置为了x0
    csrw    mscratch, t6
    # t5是当前任务的上下文 120(t5)是当前任务的上下文中的t6寄存器 也就将下一个任务的上下文地址存入当前任务上下文的t6中
    sw  t6, 120(t5)

    # 以上代码实现了 保存当前任务上下文 

1:
    # 标记下次个任务的上下文 让mscratch存入下一个任务的上下文
    csrw    mscratch, a0

    # 使用t6标记下一个任务的上下文 然后恢复下一个任务的所有上下文 
    mv  t6, a0
    reg_restore  t6

    # 下一个任务在上次调用switch_to时 会把下一条指令的地址存入ra寄存器
    # 随着寄存器的恢复 这里的ret会跳转代码到上次调用switch_to时的下一条指令
    ret


.end
