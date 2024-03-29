#pragma once

#include <stdio.h>

/**
 * Set Interrupt Flag
 * https://www.felixcloutier.com/x86/sti
 */
static inline void interrupt_set_flag() {
    printf("Set Interrupt Flag\n");
    __asm__ volatile("sti");
}

/**
 * Clear Interrupt Flag
 * https://www.felixcloutier.com/x86/cli
 */
static inline void interrupt_clear_flag() {
    printf("CLear Interrupt Flag\n");
    __asm__ volatile("cli");
}
