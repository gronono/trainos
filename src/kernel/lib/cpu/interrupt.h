#pragma once

#include "../typedefs.h"
#include "../debug/debug.h"
#include "idt/idt.h"

/**
 * Set Interrupt Flag
 * https://www.felixcloutier.com/x86/sti
 */
static inline void set_interrupt_flag() {
    debug("Set Interrupt Flag\n");
    __asm__ volatile("sti");
}

/**
 * Clear Interrupt Flag
 * https://www.felixcloutier.com/x86/cli
 */
static inline void clear_interrupt_flag() {
    debug("CLear Interrupt Flag\n");
    __asm__ volatile("cli");
}
