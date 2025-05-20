#define UART0_BASE     0x44E09000
#define UART_THR       (UART0_BASE + 0x00)
#define UART_LSR       (UART0_BASE + 0x14)
#define UART_LSR_THRE  0x20

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

/*  UART RELATED FUNCTIONS
    -- uart_send: send a character to the UART
    -- uart_puts: send a string to the UART
    -- uart_putnum: send a number to the UART (not used in this OS)
    -- uart_puthex: send a hexadecimal number to the UART (not used in this OS)
*/

void uart_send(unsigned char x) {
    while ((GET32(UART_LSR) & UART_LSR_THRE) == 0);
    PUT32(UART_THR, x);
}

void uart_puts(const char *s) {
    while (*s) {
        uart_send(*s++);
    }
}

void uart_putnum(unsigned int num) {
    char buf[10];
    int i = 0;
    do {
        buf[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);
    while (i > 0) {
        uart_send(buf[--i]);
    }
    uart_send('\n');
}

void uart_puthex(unsigned int num) {
    const char *hex = "0123456789ABCDEF";
    uart_puts("0x");
    for (int i = 28; i >= 0; i -= 4) {
        uart_send(hex[(num >> i) & 0xF]);
    }
    uart_send('\n');
}