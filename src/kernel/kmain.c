#include "lib/console/console.h"
#include "lib/debug/debug.h"

__attribute__((section(".kmain"))) void kmain() {
    debug_init();

    BREAKPOINT
    debug("Starting Kernel...\n");
    BREAKPOINT

    // Never returns to booloader
    for (;;) {}
}
