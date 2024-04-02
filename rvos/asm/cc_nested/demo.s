# void _start()
# {
#     // calling nested routine
#     aa_bb(3, 4);
# }
#
# int aa_bb(int a, int b)
# {
#     return square(a) + square(b);
# }
#
# int square(int num)
# {
#     return num * num;
# }

.text
.global _start

_start:
	la sp, stack_end		# 设置栈指针
	
	# aa_bb(3, 4);
	li a0, 3			# 使用a0和a1保存参数
	li a1, 4
	
	call aa_bb

stop:
	j stop

# int aa_bb(int a, int b)
# return a^2 + b^2
aa_bb:
	# prologue
	addi sp, sp, -16 		# 当前函数的栈预留了4个字节 因为以下寄存器有以下作用 
	sw s0, 0(sp)			# 参数1传进来s0
	sw s1, 4(sp)			# 参数2传进来s1
	sw s2, 8(sp)			# 两个函数返回值的结果加和存进来s2
	sw ra, 12(sp)			# 当前函数栈的上一层函数的返回地址

	mv s0, a0			# 从a0中取值传给s0 argument寄存器
	mv s1, a1			# 从a1中取值给s1

	li s2, 0			# 初始化返回值的结果为0

	mv a0, s0			# 参数寄存器a0这时候要存入新的值了因为下一层函数调用要用参数
	jal square
	add s2, s2, a0			# 默认用a0存返回值 所以这里s2 = s2 + a0
	
	mv a0, s1
	jal square
	add s2, s2, a0

	mv a0, s2			# aa_bb函数的返回值是s2也存入a0中

	# epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	ret

# int square(int num)
square:
	addi sp, sp, -8
	sw s0, 0(sp)
	sw s1, 4(sp)
	
	mv s0, a0
	mul s1, s0, s0
	mv a0, s1

	lw s0, 0(sp)
	lw s1, 4(sp)
	addi sp, sp, 8
	
	ret
	
	nop


stack_start:
	.rept 12
	.word 0
	.endr
stack_end:

.end


