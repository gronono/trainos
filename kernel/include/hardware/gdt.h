/**
 * Global Descriptor Table (GDT) définitions.
 * GDT defines memory areas called segments.
 * Each segments has some flags to indicate if the area is readable, writable or executable.
 *
 * Because we must setup the GDT before entering in protected mode,
 * the GDT is initialized by our Volume Boot Root (VBR).
 * It define two GDT entries map on the 1st MB memory.
 * The fist one (GDT_KERNEL_CODE) located at offset 0x08 is executable but no writable
 * The second one (GDT_KERNEL_DATA° located at offset 0x10 is writable but no executable.
 */
#pragma once

#include <stdint.h>

/**
 * Offset value for an entry in the GDT.
 */
typedef uint16_t GDTOffset_t;

/** GDT offset for kernel code segment. */
#define GDT_KERNEL_CODE         0x08

/** GDT offset for kernel data segment. */
#define GDT_KERNEL_DATA         0x10
