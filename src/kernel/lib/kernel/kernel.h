#pragma once
#include "../varargs.h"

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

void kernel_init();

void kprintf(const char* format, ...);
void kprintf_vargs(const char* format, va_list vargs);
void panic(const char* format, ...);
void halt();
