#include "uart.h"
#include "timer.h"

extern void store_context(unsigned int *ctx);
extern void load_context(unsigned int *ctx);
extern void PUT32(unsigned int, unsigned int);

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

#define NUM_PROGRAMS 2

extern unsigned int *context_queue[NUM_PROGRAMS];
extern int queue_head;

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

