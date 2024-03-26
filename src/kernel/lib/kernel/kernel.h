#pragma once

void kernel_init();

void kprintf(const char* format, ...);
void panic(const char* format, ...);
void halt();

typedef void (*PutCharFunction)(char);
void set_putc(PutCharFunction putc);
