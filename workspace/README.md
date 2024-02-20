todo:

- [ ] [实验信息概览 - 计算机组成原理（2021年） (tsinghua.edu.cn)](https://lab.cs.tsinghua.edu.cn/cod-lab-docs/labs/1-overview/)

未解之谜 

![image-20240217230148914](.\images\image-20240217230148914.png)

源代码：

```c
#include <stdio.h>
int global_init = 0x11111111;
const int global_const = 0x22222222;
void main()
{
 static int static_var = 0x33333333;
 static int static_var_uninit;
 int auto_var = 0x44444444;
 printf("hello world!\n");
 auto_var = static_var;
 return;
}
```

重定向表说.rodata+0 .rodata在000074处

可是二进制文件查看000074处的代码时2222 2222而不是hello world



编译application的时候

用riscv64-linux-gun-gcc 命令 这个命令带标准库

如何用qemu-riscv64-static命令运行app

缺少动态库的 去/usr下面找riscv64-linux-gun-gcc 下的lib拷贝到/lib

记得记录这个问题





workspace记录笔记

os目录中开一个文件夹记录作业

均由git仓库管理

