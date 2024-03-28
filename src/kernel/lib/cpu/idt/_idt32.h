#pragma once

#include "../../typedefs.h"
#include "../gdt.h"

/**
 * Represents an entry in the Interrupt Descriptor Table (IDT) in 32-bits mode.
 */
typedef struct {
    /** Lower 16 bits of the Interrupt Service Routine (ISR) address in gdt_segment. */
    uint16_t isr_low;
    /** GDT segment selector for the ISR. */
    GDTOffset gdt_segment;
    /** Reserved field, should be set to zero. */
    uint8_t  reserved;
    /** Flags specifying the type and attributes of the interrupt gate. */
    uint8_t  flags;
    /** Upper 16 bits of the ISR address. */
    uint16_t isr_high;
} __attribute__((packed)) IDTEntry;

// Gate Task is only available on x86 (32bits)
#define IDT_GATE_TASK           0b00000101

void idt_set_entry(IDTEntry* idt_entries, uint8_t index, void* handler, uint8_t flags) {
    IDTEntry* entry = &idt_entries[index];
    entry->isr_low = ((size_t) handler) & 0xFFFF;
    entry->isr_high = ((size_t) handler) >> 16 & 0xFFFF;
    entry->gdt_segment = GDT_KERNEL_CODE;
    entry->flags = flags;
    entry->reserved = 0;
}