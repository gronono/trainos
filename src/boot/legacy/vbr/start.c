void start() {
    // Breakpoint Bochs to test that C code works
    __asm__("xchgw %bx, %bx");
    for (;;);
}
