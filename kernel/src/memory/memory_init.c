#include <memory/memory.h>

extern print_memory_map(MemoryMap* map);
void memory_init(MemoryMap* map) {
    print_memory_map(map);
}
