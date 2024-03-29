#pragma once

#include <stdint.h>

/**
 * Typedef representing an offset value for an entry in the Global Descriptor Table (GDT).
 */
typedef uint16_t GDTOffset;

/** Constant representing the GDT entry for kernel code segment. */
#define GDT_KERNEL_CODE         0x08

/** Constant representing the GDT entry for kernel data segment. */
#define GDT_KERNEL_DATA         0x10
