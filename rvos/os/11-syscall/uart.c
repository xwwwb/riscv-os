#include "types.h"
#include "platform.h"

/*
 * Reference
 * [1]: TECHNICAL DATA ON 16550, http://byterunner.com/16550.html
 */

// LSR第五位1或者0
// 1 transmitter hold register (or FIFO) is empty. CPU can load the next character.
// 0 transmit holding register is full. 16550 will not accept any data for transmission.
#define LSR_TX_IDLE (1 << 5)
// LSR第一位0或者1
// 0 = no data in receive holding register or FIFO.
// 1 = data has been receive and saved in the receive holding register or FIFO.
#define LSR_RX_IDLE (1 << 0)

// 定义寄存器
struct uart_16550a_regs
{
    uint8_t RBR_THR_DLL; // 0x00 Receiver Buffer Register / Transmitter Holding Register / Divisor Latch LSB
    uint8_t IER_DLM;     // 0x01 Interrupt EnableRegister / Divisor Latch MSB
    uint8_t IIR_FCR;     // 0x02 Interrupt Status Register / FIFO Control Register
    uint8_t LCR;         // 0x03 Line Control Register
    uint8_t MCR;         // 0x04 Modem Control Register
    uint8_t LSR;         // 0x05 LineStatus Register
    uint8_t MSR;         // 0x06 Modem Status Register
    uint8_t SPR;         // 0x07 ScratchPad Register
};

/*
 * POWER UP DEFAULTS
 * IER = 0
 * ISR = 1
 * LCR = 0
 * MCR = 0
 * LSR = 60 HEX
 * MSR = BITS 0-3 = 0, BITS 4-7 = inputs
 * FCR = 0
 * TX = High
 * OP1 = High
 * OP2 = High
 * RTS = High
 * DTR = High
 * RXRDY = High
 * TXRDY = Low
 * INT = Low
 */

// 创建结构体指针指向uart的映射地址
volatile static struct uart_16550a_regs *regs = (struct uart_16550a_regs *)UART0;

void uart_init()
{

    // 禁用中断 避免没有初始化完成就发生中断
    regs->IER_DLM = 0;

    // LCR的第7位设置为1 使得前两个寄存器可以用来修改波特率
    // LCR BIT 7:
    // The internal baud rate counter latch enable (DLAB).

    // BIT 7 BAUD LATCH
    // 0 disabled, normal operation
    // 1 enabled
    uint8_t lcr = regs->LCR;
    regs->LCR = lcr | (1 << 7); // 1000000

    /*
     * Setting baud rate. Just a demo here if we care about the divisor,
     * but for our purpose [QEMU-virt], this doesn't really do anything.
     *
     * Notice that the divisor register DLL (divisor latch least) and DLM (divisor
     * latch most) have the same base address as the receiver/transmitter and the
     * interrupt enable register. To change what the base address points to, we
     * open the "divisor latch" by writing 1 into the Divisor Latch Access Bit
     * (DLAB), which is bit index 7 of the Line Control Register (LCR).
     *
     * Regarding the baud rate value, see [1] "BAUD RATE GENERATOR PROGRAMMING TABLE".
     * We use 38.4K when 1.8432 MHZ crystal, so the corresponding value is 3.
     * And due to the divisor register is two bytes (16 bits), so we need to
     * split the value of 3(0x0003) into two bytes, DLL stores the low byte,
     * DLM stores the high byte.
     */
    // 写入DLL和DLM寄存器存入数字0x0003
    regs->RBR_THR_DLL = 0x03;
    regs->IER_DLM = 0x00;

    lcr = 0;
    regs->LCR = lcr | (3 << 0);
    // 此时lcr是 00000011
    // 数据长度为8
    // 停止位1位
    // no parity 不校验
    // no break control
    // 关闭波特率的开关

    // 打开信息接受中断
    // IER 寄存器的第0位设置为1 enable the receiver ready interrupt
    regs->IER_DLM = regs->IER_DLM | (1 << 0);
}

int uart_putc(char ch)
{
    // 使用轮询的方式
    // 当LSR的寄存器第五位和10000与不为0的时候发送
    // 如果LSR第五位为0 则与的结果始终为0
    // 当第五位为1的时候 与的结果为10000 跳出循环
    while (!(regs->LSR & LSR_TX_IDLE))
        ;
    return regs->RBR_THR_DLL = ch;
}

void uart_puts(char *s)
{
    while (*s)
    {
        uart_putc(*(s++));
    }
}
int uart_getc()
{
    // LSR中第0位为1时 表示收到了消息保存到了寄存器
    if (regs->LSR & LSR_RX_IDLE)
    {
        // 从RBR中拿到字符
        return regs->RBR_THR_DLL;
    }
    else
    {
        return -1;
    }
}

/** 收到了uart中断 进入这个函数处理中断 */
void uart_receive()
{
    while (1)
    {
        int c = uart_getc();
        if (c == -1)
        {
            break;
        }
        else
        {
            // 打印字符
            uart_putc(c);
            uart_putc('\n');
        }
    }
}
