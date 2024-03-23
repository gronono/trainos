#ifndef TRAINOS_MBR_H
#define TRAINOS_MBR_H

#include <stdint.h>
#include <stdbool.h>

#define MBR_SECTOR_SIZE 512
#define MBR_SIZE MBR_SECTOR_SIZE

struct ChsAddr {
    uint8_t cylinder;
    uint8_t head;
    uint8_t sector;
} __attribute__((packed));

#define MBR_BOOT_FLAG   0x80

struct PartitionEntry {
    uint8_t bootFlag;
    struct ChsAddr chsStart;
    uint8_t type;
    struct ChsAddr chsLastSector;
    uint32_t lbaStart;
    uint32_t numberSectors;
} __attribute__((packed));

#define MBR_BOOT_SIGNATURE 0xAA55

struct MasterBootRecord {
    uint8_t bootstrap[440];
    uint32_t diskId;
    uint16_t reserved;
    struct PartitionEntry partitions[4];
    uint16_t bootSignature;
} __attribute__((packed));

struct MasterBootRecord* mbr_load(const char* diskPath);
char* mbr_decode_partition_type(const struct PartitionEntry* partition);
bool mbr_is_partition_bootable(const struct PartitionEntry* partition);
bool mbr_is_valid_boot_signature(uint16_t bootSignature);

#endif //TRAINOS_MBR_H
