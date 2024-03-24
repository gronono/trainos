#pragma once

#include "../typedefs.h"

/**
 * Set Interrupt Flag
 * https://www.felixcloutier.com/x86/sti
 */
static inline void set_interrupt_flag() {
    __asm__("sti");
}

/**
 * Clear Interrupt Flag
 * https://www.felixcloutier.com/x86/cli
 */
static inline void clear_interrupt_flag() {
 __asm("cli");
}
