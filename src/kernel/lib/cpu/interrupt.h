#pragma once

#include "../typedefs.h"
#include "idt/idt.h"
#include "../kernel/kernel.h"

/**
 * Set Interrupt Flag
 * https://www.felixcloutier.com/x86/sti
 */
static inline void interrupt_set_flag() {
    kprintf("Set Interrupt Flag\n");
    __asm__ volatile("sti");
}

/**
 * Clear Interrupt Flag
 * https://www.felixcloutier.com/x86/cli
 */
static inline void interrupt_clear_flag() {
    kprintf("CLear Interrupt Flag\n");
    __asm__ volatile("cli");
}
