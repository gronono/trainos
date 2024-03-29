#pragma once

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

extern void* kernel_start;
extern void* kernel_end;

void kernel_init();

void kernel_panic(const char* format, ...);
void kernel_halt();

