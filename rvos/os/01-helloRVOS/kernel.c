extern void uart_init(void);
extern void uart_puts(char *string);
void start_kernel(void)
{
    uart_init();
    uart_puts("Hello, RISC-V OS!\n");
    uart_puts("author: xwwwb\n");
    while (1)
    {
    }
}