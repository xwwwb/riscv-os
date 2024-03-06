Todo:

**riscv 操作系统**

- [ ] [实验信息概览 - 计算机组成原理（2021 年） (tsinghua.edu.cn)](https://lab.cs.tsinghua.edu.cn/cod-lab-docs/labs/1-overview/)
- [ ] Xv6
- [ ] Linux 内核实现
- [ ] 买的操作系统网课

**riscv 驱动开发**

- [ ] RISC-V 体系结构编程与实践

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

**windows 开发**

- [ ] win32 开发

**Linux 开发**

- [ ] Linux API

**Linux 使用**

- [ ] 鸟哥
- [ ] Linux 就该这么学

**编译器开发**

**模拟器开发**

- [ ] https://weijiew.github.io/everystep/crvemu/README

**fpga 开发**

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

重定向表说.rodata+0 .rodata 在 000074 处

可是二进制文件查看 000074 处的代码时 2222 2222 而不是 hello world

编译 application 的时候

用 riscv64-linux-gun-gcc 命令 这个命令带标准库

如何用 qemu-riscv64-static 命令运行 app

缺少动态库的 去/usr 下面找 riscv64-linux-gun-gcc 下的 lib 拷贝到/lib

记得记录这个问题

workspace 记录笔记

os 目录中开一个文件夹记录作业

均由 git 仓库管理
