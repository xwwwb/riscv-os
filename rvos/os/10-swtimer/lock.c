#include "os.h"

int spin_lock()
{
    w_mstatus(r_mstatus() & ~MSTATUS_MIE);
    return 0;
}

int spin_unlock()
{
    // 开中断
    w_mstatus(r_mstatus() | MSTATUS_MIE);
    return 0;
}