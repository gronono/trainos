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

// TODO x86_64
// on x86_64 IDT are larger
// https://wiki.osdev.org/IDT

struct InterruptDescriptor {
    uint16_t offset_1;        // offset bits 0..15
    uint16_t selector;        // a code segment selector in GDT or LDT
    uint8_t  zero;            // unused, set to 0
    uint8_t  type_attributes; // gate type, dpl, and p fields
    uint16_t offset_2;        // offset bits 16..31
};

struct InterruptDescriptor interrupt_description_table[256];
