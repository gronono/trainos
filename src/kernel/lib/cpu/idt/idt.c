#include "idt.h"
#include "../interrupt.h"
#include "_isr.h"
#include "../bios.h"
#include "../../kernel/kernel.h"

#ifdef __x86_64__
#include "_idt64.h"
#else
#include "_idt32.h"
#endif

#define IDT_GATE_INTERRUPT      0b00001110
#define IDT_GATE_TRAP           0b00001111
#define IDT_RING_0              0b00000000
#define IDT_RING_1              0b00100000
#define IDT_RING_2              0b01000000
#define IDT_RING_3              0b01100000
#define IDT_PRESENT             0b10000000

// Number of entries into IDT
#define IDT_ENTRIES_SIZE        256

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

/**
 * Represents the context of a CPU when an interrupt or exception occurs.
 */
struct InterruptFrame {
    /** Control Register 2 value at the time of interrupt/exception. */
    size_t page_fault_address;
    /** Data Segment value at the time of interrupt/exception. */
    size_t data_segment;
    /** RDI / EDI register value. */
    size_t destination_register;
    /** RSI / ESI register value. */
    size_t source_register;
    /** RBP / EBP register value. */
    size_t stack_base_pointer;
    /** RSP / ESP register value. */
    size_t stack_pointer;
    /** RBX / EBX register value. */
    size_t base_register;
    /** RDX / EDX register value. */
    size_t data_register;
    /** RCX / ECX register value. */
    size_t counter_register;
    /** RAX / EAX register value. */
    size_t accumulator_register;
    /** Interrupt number or exception vector. */
    size_t idt_index;
    /** Error code (if applicable) for exceptions with error codes. */
    size_t error_code;
    /** Instruction pointer at the time of interrupt/exception. */
    size_t intruction_pointer;
    /** Code Segment value at the time of interrupt/exception. */
    size_t code_segment;
    /** EFLAGS register value. */
    size_t eflags;
    /** User-mode stack pointer value (if in user-mode). */
    size_t useresp;
    /** Stack Segment value at the time of interrupt/exception. */
    size_t stack_segment;
} __attribute__((packed));

/**
 * Array representing the Interrupt Descriptor Table (IDT).
 *
 * This array holds all the entries for the IDT, which are structures of type `IDTEntry`.
 * The IDT is used for managing interrupt handlers and their respective entry points.
 * The number of entries is 256 (0-255) for all possible interrupt vectors.
 */
struct IDTEntry idt_entries[IDT_ENTRIES_SIZE];

/**
 * Handler function for Interrupt Service Routines (ISR).
 *
 * This function is called when an interrupt occurs. It checks if the interrupt
 * is an exception (IDT index < 32) and panics with an error message containing
 * the corresponding exception description.
 *
 * @param frame Pointer to the InterruptFrame containing CPU context at the time of interrupt.
 */
void isr_handler(struct InterruptFrame* frame){
    if (frame->idt_index < 32){
        panic("!! Exception: %d (%s) - error: 0x%X !!\n",
              frame->idt_index,
              exception_messages[frame->idt_index],
              frame->error_code);
    } else {
        panic("!! Unknown interrupt: %d !!\n", frame->idt_index);
    }
}

void *irq_routines[] = {
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0
};

void irq_handler(struct InterruptFrame* frame){
    void (*handler)(struct InterruptFrame *frame);

    uint16_t idt_index = frame->idt_index - 32;
    handler = irq_routines[idt_index];

    if (handler) {
        kprintf("IRQ %d: %p\n", idt_index, handler);
        handler(frame);
    }

    // TODO try to understand
    if (frame->idt_index >= 40) {
        port_write(0xA0, 0x20);
    }

    port_write(0x20,0x20);
}

void init_idt() {
    kprintf("Init IDT - addr: %p\n", idt_entries);
    clear_interrupt_flag();

    // TODO understand !!
    //0x20 commands and 0x21 data
    //0xA0 commands and 0xA1 data
    port_write(0x20, 0x11);
    port_write(0xA0, 0x11);
    port_write(0x21, 0x20);
    port_write(0xA1, 0x28);
    port_write(0x21,0x04);
    port_write(0xA1,0x02);
    port_write(0x21, 0x01);
    port_write(0xA1, 0x01);
    port_write(0x21, 0x0);
    port_write(0xA1, 0x0);

    // Exception Handlers
    for (uint8_t idt_index = 0; idt_index < 32; idt_index++) {
        uint8_t flags = IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP;
        idt_set_entry(idt_entries, idt_index, handlers[idt_index], flags);
    }
    // IRQ Handlers
    for (uint8_t idt_index = 32; idt_index < 48; idt_index++) {
        uint8_t flags = IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT;
        idt_set_entry(idt_entries, idt_index, handlers[idt_index], flags);
    }
    // Others handlers
    for (uint16_t idt_index = 48; idt_index < IDT_ENTRIES_SIZE; idt_index++) {
        uint8_t flags = IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT;
        idt_set_entry(idt_entries, idt_index, handlers[idt_index], flags);
    }
    // Syscall interrupts
    idt_set_entry(idt_entries, 128, isr128, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 177, isr177, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);

    struct IDTDescriptor descriptor;
    descriptor.size = sizeof(idt_entries);
    descriptor.ptr_table = idt_entries;
    kprintf("Set IDT Description. Size=%d. Table=%p\n", descriptor.size, descriptor.ptr_table);
    __asm__ volatile("lidt %0" : : "m" (descriptor));

    set_interrupt_flag();
}
