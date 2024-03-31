#pragma once

#include <stdint.h>

typedef struct {
    uint64_t start_address;
    uint64_t length;
    uint32_t type;
} __attribute__ ((packed)) MemoryMapEntry;

typedef struct {
    uint32_t size;
    MemoryMapEntry entries[];
} __attribute__ ((packed)) MemoryMap;

void memory_init(MemoryMap* map);
