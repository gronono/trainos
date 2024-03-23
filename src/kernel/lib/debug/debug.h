#pragma once

#define BREAKPOINT __asm__("xchgw %bx, %bx");

void debug_init();
void debug(const char* format, ...);