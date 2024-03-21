$(warning Start)
EXEC = demo

SRC = ${EXEC}.s

# 这里为什么引入的是上层的文件 难道是该build.mk被子文件夹中的makefile引用了
GDBINIT = ../gdbinit

include ../rule.mk