#pragma once

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

void kernel_init();

void kprintf(const char* format, ...);
void panic(const char* format, ...);
void halt();
