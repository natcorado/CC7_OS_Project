extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void enable_irq(void);

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

typedef struct {
    unsigned int pid;
    unsigned int sp;
    unsigned int state;

} PCB;

PCB pcb_array[2];

unsigned int seed = 12345;

unsigned int rand(void) {
    seed = (seed * 1103515245 + 12345) & 0x7fffffff;
    return seed;
}

void delay(void) {
    for(volatile int i = 0; i < 10000; i++) { }
}

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

/*  TIMER RELATED FUNCTIONS
    -- timer_init: initialize the timer
    -- timer_irq_handler: handle the timer interrupt
*/

void timer_init(void) {
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    PUT32(INTC_MIR_CLEAR2, 1 << (68 - 64));
    PUT32(INTCPS_BASE + 0x110, 0x0);  

    PUT32(TCLR, 0);

    PUT32(TISR, 0x7);

    PUT32(TLDR, 0xFFFF0000);  
    PUT32(TCRR, 0xFFFF0000); 

    PUT32(TIER, 0x2);

    PUT32(TCLR, 0x3);
}


/* CPSR -> Current program status register
   SPSR -> Saved program status register: The SPSR is a 
   copy of the CPSR that gets automatically saved when:
   an interrupt occurs ????? :,) why?

*/
void timer_irq_handler(void) {

    PUT32(TISR, 0x2);
    
    
    uart_send('!');
    uart_send('\r');
    uart_send('\n');

    asm volatile (
        "stmfd sp!, {r0-r12, lr};\n"  
        "mrs r0, spsr;\n"              
        "stmfd sp!, {r0};\n"          
    );
    
    PUT32(INTC_CONTROL, 0x1);

    while (1) {
    }
}





