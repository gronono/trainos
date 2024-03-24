#include "idt.h"
#include "../debug/debug.h"
#include "interrupt.h"
#include "../../kernel.h"

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
} __attribute__((packed));

struct InterruptFrame {
    /** Address where the interrupt occurred */
    uint32_t instruction_pointer;
    /** Code segment in GDT */
    uint32_t  code_segment;
    uint32_t  flags;
    /** Current stack address */
    uint32_t  stack_pointer;
    /** Stack segment in GDT */
    uint32_t  stack_segment;
} __attribute__((packed));

/**
 * Array representing the Interrupt Descriptor Table (IDT).
 *
 * This array holds all the entries for the IDT, which are structures of type `IDTEntry`.
 * The IDT is used for managing interrupt handlers and their respective entry points.
 * The number of entries is 256 (0-255) for all possible interrupt vectors.
 */
struct IDTEntry idt_entries[256];

void __attribute__((interrupt)) default_exception_handler(struct InterruptFrame* frame) {
    panic("Default Exception Handler at %p\n", frame->instruction_pointer);
}

void __attribute__((interrupt)) default_exception_handler_error_code(struct InterruptFrame* frame) {
    panic("Default Exception Handler with error at %p\n", frame->instruction_pointer);
}

void __attribute__((interrupt)) default_interrupt_handler(struct InterruptFrame* frame) {
    panic("Default Exception Handler at %p\n", frame->instruction_pointer);
}

void init_idt() {
    debug("Init IDT - addr: %p\n", idt_entries);
    clear_interrupt_flag();

    // Set exceptions handlers
    for (uint8_t i = 0; i < 32; i++) {
        if (i == 8  || i == 10 || i == 11 || i == 12 ||
            i == 13 || i == 14 || i == 17 || i == 21) {
            // Exception takes an error code
            idt_set_entry(idt_entries, i, default_exception_handler_error_code, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
        } else {
            idt_set_entry(idt_entries, i, default_exception_handler, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
        }
    }

    // Set regular interrupts handlers
    for (uint16_t i = 32; i < 256; i++) {
        idt_set_entry(idt_entries, i, default_interrupt_handler, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    }

    struct IDTDescriptor descriptor;
    descriptor.size = sizeof(idt_entries);
    descriptor.ptr_table = idt_entries;
    debug("Set IDT Description. Size=%d. Table=%p\n", descriptor.size, descriptor.ptr_table);
    __asm__ volatile("lidt %0" : : "m" (descriptor));

    set_interrupt_flag();
}
