#pragma once

#include <stdint.h>
#include "../gdt.h"

/**
 * Represents an entry in the Interrupt Descriptor Table (IDT) in 64-bit mode.
 */
typedef struct {
    /** Lower 16 bits of the Interrupt Service Routine (ISR) address. */
    uint16_t isr_low;
    /** GDT segment selector for the ISR. */
    GDTOffset gdt_segment;
    /** Reserved field, should be set to zero. */
    uint8_t reserved1;
    /** Flags specifying the type and attributes of the interrupt gate. */
    uint8_t flags;
    /** Middle 32 bits of the ISR address. */
    uint32_t isr_middle;
    /** Upper 32 bits of the ISR address. */
    uint32_t isr_high;
    /** Reserved field, should be set to zero. */
    uint32_t reserved2;
} __attribute__((packed)) IDTEntry;

void idt_set_entry(IDTEntry* idt_entries, uint16_t index, void* handler, uint8_t flags) {
    IDTEntry entry = idt_entries[index];
    entry.isr_low = ((uintptr_t) handler) & 0xFFFF;
    entry.isr_middle = ((uintptr_t) handler) >> 16 & 0xFFFF;
    entry.isr_high = ((uintptr_t) handler) >> 32 & 0xFFFF;
    entry.gdt_segment = GDT_KERNEL_CODE;
    entry.flags = flags;
    entry.reserved1 = 0;
    entry.reserved2 = 0;
}
