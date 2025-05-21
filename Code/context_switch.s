.section .text
.syntax unified
.code 32
.extern uart_send

// void store_context(unsigned int *ctx)
// r0 = pointer to context array
.global store_context
store_context:
    stmia   r0, {r0-r12, sp, lr}
    bx      lr

// void load_context(unsigned int *ctx)
// Loads r0-r12, lr, and spsr from the memory pointed by ctx
.global load_context
load_context:
    ldmia   r0, {r0-r12, sp, lr}
    bx      lr  
