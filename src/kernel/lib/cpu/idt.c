#include "idt.h"
#include "../debug/debug.h"
#include "interrupt.h"

#ifdef __x86_64__
#include "idt64.h"
#else
#include "idt32.h"
#endif

#define IDT_GATE_INTERRUPT      0b00001110
#define IDT_GATE_TRAP           0b00001111
#define IDT_RING_0              0b00000000
#define IDT_RING_1              0b00100000
#define IDT_RING_2              0b01000000
#define IDT_RING_3              0b01100000
#define IDT_PRESENT             0b10000000

/**
 * Represents the Interrupt Descriptor Table (IDT) Descriptor.
 * This structure is used to define the layout of the IDT Descriptor,
 * which is used to load the Interrupt Descriptor Table (IDT) into memory.
 */
struct IDTDescriptor {
    /** The size of the IDT (total number of entries) - 1. */
    uint16_t size;
    /** Pointer to the first entry of the IDT. */
    struct IDTEntry * ptr_table;
};

/**
 * Array representing the Interrupt Descriptor Table (IDT).
 *
 * This array holds all the entries for the IDT, which are structures of type `IDTEntry`.
 * The IDT is used for managing interrupt handlers and their respective entry points.
 * The number of entries is 256 (0-255) for all possible interrupt vectors.
 */
struct IDTEntry idt_entries[256];

void default_exception_handler() {
    debug("Default Exception Handler");
}

void default_interrupt_handler() {
    debug("Default Exception Handler");
}

void init_idt() {
    debug("Init IDT - addr: %p\n", idt_entries);
    clear_interrupt_flag();

    // Set exceptions handlers
    for (uint8_t i = 0; i < 32; i++) {
        idt_set_entry(i, default_exception_handler, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    }
    // Set regular interrupts handlers
    for (uint8_t i = 32; i <= 255; i++) {
        idt_set_entry(i, default_interrupt_handler, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    }

    struct IDTDescriptor descriptor;
    descriptor.size = sizeof(idt_entries) - 1;
    descriptor.ptr_table = idt_entries;

    __asm__ volatile("lidt %0" : : "m" (descriptor));

    set_interrupt_flag();
}
