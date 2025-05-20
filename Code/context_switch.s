.section .text
.syntax unified
.code 32
.global return_register
.global load_context
.extern uart_send

// void return_register(int registerNum)
// returns register 
return_register:

    bx lr

// void load_context(unsigned int *ctx)
// Loads r0-r12, lr, and spsr from the memory pointed by ctx
load_context:
    mov     r1, r0
    ldmia   r1, {r0-r12, lr}
    ldr     r2, [r1, #56]
    msr     spsr_cxsf, r2
    bx      lr
