#ifndef OS_H
#define OS_H

// Function declarations
extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void enable_irq(void);

// Hardware registers
#define UART0_BASE     0x44E09000
#define UART_THR       (UART0_BASE + 0x00)
#define UART_LSR       (UART0_BASE + 0x14)
#define UART_LSR_THRE  0x20

#define DMTIMER2_BASE    0x48040000
#define TCLR             (DMTIMER2_BASE + 0x38)
#define TCRR             (DMTIMER2_BASE + 0x3C)
#define TISR             (DMTIMER2_BASE + 0x28)
#define TIER             (DMTIMER2_BASE + 0x2C)
#define TLDR             (DMTIMER2_BASE + 0x40)

#define INTCPS_BASE      0x48200000
#define INTC_MIR_CLEAR2  (INTCPS_BASE + 0xC8)
#define INTC_CONTROL     (INTCPS_BASE + 0x48)

#define CM_PER_BASE      0x44E00000
#define CM_PER_TIMER2_CLKCTRL (CM_PER_BASE + 0x80)

// Function prototypes
void uart_send(unsigned char x);
void uart_puts(const char *s);
void uart_putnum(unsigned int num);
void uart_puthex(unsigned int num);
void timer_init(void);
void timer_irq_handler(void);
unsigned int rand(void);

#endif /* OS_H */ 