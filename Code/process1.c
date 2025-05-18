
extern void uart_send(unsigned char);
extern void enable_irq(void);
extern void timer_irq_handler(void);
extern void timer_init(void);
extern void uart_puts(const char *s);

int main(void)
{
    char c;
    timer_init();
    enable_irq();
 
    while (1)
    {
        for (c = '0'; c <= '9'; c++)
        {
            uart_send(c);
            uart_send('\n');
        }
    }
    return 0;
}