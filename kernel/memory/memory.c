#include "memory.h"

#include <stdint.h>
#include <stdio.h>

#define MEMORY_MAP_ADDR 0x8000

typedef struct {
    uint64_t start_address;
    uint64_t length;
    uint32_t type;
} __attribute__ ((packed)) MemoryMapEntry;

typedef struct {
    uint32_t size;
    MemoryMapEntry entries[];
} __attribute__ ((packed)) MemoryMap;

void print_memory_map() {
    MemoryMap* map = (MemoryMap*) MEMORY_MAP_ADDR;
    printf("Memory Map from %p : %u entries\n", MEMORY_MAP_ADDR, map->size);
    printf("#  Start Address       Length              Type\n");
    uint64_t total_size = 0;
    for (uint32_t i = 0; i < map->size; i++) {
        MemoryMapEntry* entry = &(map->entries[i]);
        printf("#%u 0x%016llx  0x%016llx  %u\n", i,
                entry->start_address,
                entry->length,
                entry->type);
        total_size += entry->length;
    }
    printf("Total size: %llu Bytes\n", total_size);
    printf("Total size: %llu kB\n", total_size / 1024);
    printf("Total size: %llu MB\n", total_size / (1024 * 1024));
}
// QEMU
//#   Start Address       Length              Type
//#0 0x0000000000000000  0x000000000009fc00  1
//#1 0x000000000009fc00  0x0000000000000400  2
//#2 0x00000000000f0000  0x0000000000010000  2
//#3 0x0000000000100000  0x0000000007ee0000  1
//#4 0x0000000007fe0000  0x0000000000020000  2
//#5 0x00000000fffc0000  0x0000000000040000  2
//Total size: 134152192 Bytes
//        Total size: 131008 kB
//        Total size: 127 MB

// BOCHS
//#   Start Address       Length              Type
//#0 0x0000000000000000  0x000000000009f000  1
//#1 0x000000000009f000  0x0000000000001000  2
//#2 0x00000000000e8000  0x0000000000018000  2
//#3 0x0000000000100000  0x0000000007f00000  1
//#4 0x00000000fffc0000  0x0000000000040000  2
//Total size: 134184960 Bytes
//        Total size: 131040 kB
//        Total size: 127 MB
