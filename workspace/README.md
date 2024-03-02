Todo:

**riscv 操作系统**

- [ ] [实验信息概览 - 计算机组成原理（2021年） (tsinghua.edu.cn)](https://lab.cs.tsinghua.edu.cn/cod-lab-docs/labs/1-overview/)
- [ ] Xv6
- [ ] Linux内核实现
- [ ] 买的操作系统网课

**riscv驱动开发**

- [ ] RISC-V体系结构编程与实践

**网络协议栈**

**运行库开发**

- [ ] 程序员的自我修养

**链接 编译**

- [ ] 程序员的自我修养

**c++开发**

- [ ] C++ prime
- [ ] C++ 三件套
- [ ] Qt
- [ ] 常用第三方库

**windows开发**

- [ ] win32开发

**Linux开发**

- [ ] Linux API

**Linux使用**

- [ ] 鸟哥
- [ ] Linux就该这么学

**编译器开发**

**模拟器开发**

**fpga开发**



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

