# void _start()
# {
#     // calling leaf routine
#     square(3);
# }
#
# int square(int num)
# {
#     return num * num;
# }

.text
.global _start

_start:
	la sp, stack_end	# 把栈底放到stack point寄存器中 也就是约定里的x2寄存器
	
	li a0, 3		# a0也就是x10寄存器 一般用来存函数参数
	call square		# 调用函数 auipc构造地址 然后使用jalr跳转过去

stop:
	j stop

# int square(int num)
square:
	# prologue
	# 减少sp的值 根据函数中saved寄存器的情况和local变量的多少开栈空间
	# 将saved寄存器的值保存到栈里
	# 如果调用了其他函数 存ra到栈里
	
	addi sp, sp, -8		# 栈指针向上移 压栈了两个int
	sw s0, 0(sp)		# 保存saved寄存器
	sw s1, 4(sp)		# 保存saved寄存器

	mv s0, a0		# 将a0也就是保存参数的寄存器中的值移动到s0寄存器中
	mul s1, s0, s0		# 做乘法保存结果到s1
	mv a0, s1		# 用a0保存返回值

	# epilogue
	# 从栈中恢复saved寄存器
	# 有需要就恢复ra寄存器
	# 增加sp的值
	# 调用ret返回
	lw s0, 0(sp)
	lw s1, 4(sp)
	addi sp, sp, 8
	
	ret			# jalr x0, 0(x1) 因为call的时候调用的jalr会把返回地址存到x1中
	
	nop			# 给gdb演示用

stack_start:	# 静态定义了内存做栈
	.rept 10
	.word 0
	.endr
stack_end:

.end

