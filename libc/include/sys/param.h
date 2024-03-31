#pragma once

#define BREAKPOINT __asm__ volatile("xchgw %bx, %bx");

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
