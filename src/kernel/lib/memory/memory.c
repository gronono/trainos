#include "memory.h"

#include "../kernel/kernel.h"
#include "../typedefs.h"

#define MEMORY_MAP_ADDR 0x8000

typedef struct {
    unsigned long long start_address;
    unsigned long long length;
    unsigned int type;
    unsigned int attributes;
} __attribute__ ((packed)) MemoryMapEntry;

void print_memory_map() {
    uint32_t nb = *(uint32_t*) MEMORY_MAP_ADDR;
    MemoryMapEntry* entries = (MemoryMapEntry*) (MEMORY_MAP_ADDR+4);
    kprintf("Memory Map: %d at %p\n", nb, &entries);
    for (uint32_t i = 0; i < nb; i++) {
        MemoryMapEntry entry = entries[i];
        kprintf("#%u at #%p: %p, %p\n", i, entry.start_address, entry.length, entry.type);
    }
}