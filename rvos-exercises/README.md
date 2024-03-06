使用 riscv64-linux-gnu-gcc 对 c 语言文件进行交叉编译

得到的可执行文件在宿主机上直接运行也可以拿到结果
是因为我安装了 qemu-riscv-static

使用 qemu 用户态模拟 qemu-riscv64-static a.out 也可以拿到结果

给 gcc 指定 32 位会出现编译出错 可能缺少一些文件
qemu 运行 a.out 可能运行不了 需要将一些动态库复制到系统 lib 目录下
