#include "memory.h"

#include "../kernel/kernel.h"
#include "../typedefs.h"

#define MEMORY_MAP_ADDR 0x8000

struct MemoryMapEntry {
    uint64_t start_address;
    uint64_t length;
    uint32_t type;
} __attribute__ ((packed));

void print_memory_map() {
    uint32_t nb = *(uint32_t *) MEMORY_MAP_ADDR;
    struct MemoryMapEntry *entries = (struct MemoryMapEntry *) (MEMORY_MAP_ADDR + sizeof(uint32_t));
    kprintf("Memory Map: %u entries at %p - size %u\n", nb, entries, sizeof(struct MemoryMapEntry));
    kprintf("#   Start Address       Length              Type\n");
    for (uint32_t i = 0; i < nb; i++) {
        struct MemoryMapEntry entry = entries[i];
        kprintf("#%u  0x%016llx  0x%016llx  %u\n", i, entry.start_address, entry.length, entry.type);
    }
}
// QEMU
//#   Start Address       Length              Type
//#0  0x0000000000000000  0x000000000009fc00  1
//#1  0x000000000009fc00  0x0000000000000400  2
//#2  0x00000000000f0000  0x0000000000010000  2
//#3  0x0000000000100000  0x000000003fee0000  1
//#4  0x000000003ffe0000  0x0000000000020000  2
//#5  0x00000000fffc0000  0x0000000000040000  2

// BOCHS
//#   Start Address       Length              Type
//#0  0x0000000000000000  0x000000000009f000  1
//#1  0x000000000009f000  0x0000000000001000  2
//#2  0x00000000000e8000  0x0000000000018000  2
//#3  0x0000000000100000  0x0000000007f00000  1
//#4  0x00000000fffc0000  0x0000000000040000  2
