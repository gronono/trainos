#include "idt.h"
#include "interrupt.h"
#include "_isr.h"
#include "_irq.h"
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
    ptr_t cr2;
    /** Data Segment value at the time of interrupt/exception. */
    ptr_t ds;
    /** EDI register value. */
    ptr_t edi;
    /** ESI register value. */
    ptr_t esi;
    /** EBP register value. */
    ptr_t ebp;
    /** ESP register value. */
    ptr_t esp;
    /** EBX register value. */
    ptr_t ebx;
    /** EDX register value. */
    ptr_t edx;
    /** ECX register value. */
    ptr_t ecx;
    /** EAX register value. */
    ptr_t eax;
    /** Interrupt number or exception vector. */
    ptr_t idt_index;
    /** Error code (if applicable) for exceptions with error codes. */
    ptr_t error_code;
    /** Instruction pointer at the time of interrupt/exception. */
    ptr_t intruction_pointer;
    /** Code Segment value at the time of interrupt/exception. */
    ptr_t code_segment;
    /** EFLAGS register value. */
    ptr_t eflags;
    /** User-mode stack pointer value (if in user-mode). */
    ptr_t useresp;
    /** Stack Segment value at the time of interrupt/exception. */
    ptr_t stack_segment;
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
        panic("!! Exception: %s !!\n", exception_messages[frame->idt_index]);
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
        debug("IRQ %d: %p\n", idt_index, handler);
        handler(frame);
    }

    // TODO try to understand
    if (frame->idt_index >= 40) {
        port_write(0xA0, 0x20);
    }

    port_write(0x20,0x20);
}

void init_idt() {
    debug("Init IDT - addr: %p\n", idt_entries);
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
    idt_set_entry(idt_entries, 0, isr0, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 1, isr1, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 2, isr2, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 3, isr3, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 4, isr4, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 5, isr5, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 6, isr6, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 7, isr7, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 8, isr8, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 9, isr9, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 10, isr10, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 11, isr11, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 12, isr12, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 13, isr13, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 14, isr14, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 15, isr15, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 16, isr16, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 17, isr17, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 18, isr18, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 19, isr19, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 20, isr20, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 21, isr21, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 22, isr22, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 23, isr23, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 24, isr24, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 25, isr25, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 26, isr26, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 27, isr27, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 28, isr28, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 29, isr29, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 30, isr30, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 31, isr31, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);

    // IRQ Handlers
    idt_set_entry(idt_entries, 32, irq0, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 33, irq1, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 34, irq2, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 35, irq3, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 36, irq4, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 37, irq5, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 38, irq6, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 39, irq7, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 40, irq8, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 41, irq9, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 42, irq10, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 43, irq11, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 44, irq12, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 45, irq13, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);
    idt_set_entry(idt_entries, 46, irq14, IDT_PRESENT | IDT_RING_0 | IDT_GATE_INTERRUPT);

    // Syscall interrupts
    idt_set_entry(idt_entries, 128, isr128, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);
    idt_set_entry(idt_entries, 177, isr177, IDT_PRESENT | IDT_RING_0 | IDT_GATE_TRAP);

    // TODO Hardware interrupts (IRQ) idt_entries from 32 to 255

    struct IDTDescriptor descriptor;
    descriptor.size = sizeof(idt_entries);
    descriptor.ptr_table = idt_entries;
    debug("Set IDT Description. Size=%d. Table=%p\n", descriptor.size, descriptor.ptr_table);
    __asm__ volatile("lidt %0" : : "m" (descriptor));

    set_interrupt_flag();
}
