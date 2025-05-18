.section .text
.syntax unified
.code 32
.globl _start

_start:
    ldr sp, =_stack_top
    ldr r0, =vector_table
    mcr p15, 0, r0, c12, c0, 0
    bl main
    b .  @ Loop here instead of hang label
    b hang

hang:
    b hang

.globl PUT32
PUT32:
    str r1, [r0]
    bx lr

.globl GET32
GET32:
    ldr r0, [r0]
    bx lr

.globl enable_irq
enable_irq:
    mrs r0, cpsr
    bic r0, r0, #0x80         @ Clear I-bit
    msr cpsr_c, r0
    bx lr

.align 5
vector_table:
    b _start
    b .
    b .
    b .
    b .
    b .
    b irq_handler
    b .

irq_handler:
    sub lr, lr, #4              @ Adjust LR to correct return address
    srsdb sp!, #0x13           @ Save return state on supervisor stack
    cpsid i                    @ Disable interrupts
    push {r0-r12, lr}         @ Save all registers
    
    @ Save SPSR
    mrs r0, spsr
    push {r0}
    
    @ Call the C interrupt handler
    bl timer_irq_handler
    
    @ Restore SPSR
    pop {r0}
    msr spsr_cxsf, r0
    
    @ Restore registers and return
    pop {r0-r12, lr}
    cpsie i                    @ Re-enable interrupts
    rfefd sp!                  @ Return from exception and restore CPSR

.section .bss
.align 4
_stack_bottom:
    .skip 0x2000             @ 8KB stack
_stack_top:
