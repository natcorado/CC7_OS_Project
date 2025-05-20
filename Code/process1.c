#include "uart.h"

extern void uart_send(unsigned char);
extern void uart_puts(const char *s);


void timer_irq_handler(void) { while (1); }

int main(void)
{
    char c;
 
    while (1)
    {
        for (c = 'A'; c <= 'Z'; c++)
        {
            uart_send(c);
            uart_send('\r');
            uart_send('\n');
        }
    }
    return 0;
}