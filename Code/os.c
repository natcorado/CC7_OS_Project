#include "uart.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);
extern void enable_irq(void);
extern void disable_irq(void);
extern void store_context(unsigned int *ctx);
extern void load_context(unsigned int *ctx);

#define NUM_PROGRAMS 2
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

unsigned int context1[17]; // Context for program 1
unsigned int context2[17]; // Context for program 2
unsigned int *context_queue[NUM_PROGRAMS] = { context1, context2 };

int queue_head = 0;

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

/*  TIMER RELATED FUNCTIONS
    -- timer_init: initialize the timer
    -- timer_irq_handler: handle the timer interrupt
*/

void timer_init(void) {
    PUT32(CM_PER_TIMER2_CLKCTRL, 0x2);

    PUT32(INTC_MIR_CLEAR2, 1 << (68 - 64));
    PUT32(INTCPS_BASE + 0x110, 0x0);  

    PUT32(TCLR, 0);                // Stop timer

    PUT32(TISR, 0x7);              // Clear interrupt flags

    PUT32(TLDR, 0xFFF00000);       // Reload value (closer to overflow = faster)
    PUT32(TCRR, 0xFFF00000);       // Set counter to same value

    PUT32(TIER, 0x2);              // Enable overflow interrupt

    PUT32(TCLR, 0x3);              // Start timer with auto-reload
}

/* CPSR -> Current program status register
   SPSR -> Saved program status register: The SPSR is a 
   copy of the CPSR that gets automatically saved when:
   an interrupt occurs ????? :,) why?
*/

void init_contexts(void) {
    // Clear all registers for both contexts
    uart_puts("Initializing contexts...\r\n");
    for (int i = 0; i < 17; ++i) {
        context1[i] = 0;
        context2[i] = 0;
    }
    uart_puts("Setting up sp and lr.\r\n");
    
    // Set up stack pointer and LR (entry point) for each program
    context1[13] = 0x8002A000;
    uart_send('1');
    context1[14] = 0x80020000;
    uart_send('2');
    context2[13] = 0x8003A000; 
    uart_send('3');
    context2[14] = 0x80030000;

    uart_puts("Contexts initialized.\r\n");
}

void timer_irq_handler(void) {
    PUT32(TISR, 0x7); // Clear all timer interrupt status flags (overflow, match, capture)
    int prev = queue_head;
    queue_head = (queue_head + 1) % NUM_PROGRAMS;
    
    uart_puts("Starting context switch...\r\n");
    store_context(context_queue[prev]);
    uart_puts("Context saved. Loading next...\r\n");    
    PUT32(INTC_CONTROL, 0x1);
    load_context(context_queue[queue_head]);

    // No return: load_context will restore and resume the next program
}

/*
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
*/

int main(void) {
    uart_puts("Simple OS starting up...\r\n");

    init_contexts();

    timer_init();
    enable_irq();

    while (1) {
        uart_puts("Main...\r\n");
    }

    return 0;
}
