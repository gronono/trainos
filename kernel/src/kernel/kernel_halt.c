#include <hardware/interrupt.h>
#include <stdio.h>

void kernel_halt() {
    printf("!! System Halt !!\n");
    interrupt_clear_flag();
    for (;;) {}
}
