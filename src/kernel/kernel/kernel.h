#pragma once

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

void kernel_init();

void kernel_panic(const char* format, ...);
void kernel_halt();

