#include "os.h"
#include "syscall.h"

int sys_gethartid()
{
    int hartid = (int)r_tp();
    return hartid;
}
void do_syscall(struct context *ctx)
{
    // 首先从a7里面拿到调用号
    int syscall_num = ctx->a7;
    switch (syscall_num)
    {
    case SYS_gethartid:
    { // 处理gethartid
        int hartid = sys_gethartid();
        ctx->a0 = hartid;
        break;
    }
    default:
        break;
    }
    return;
}
