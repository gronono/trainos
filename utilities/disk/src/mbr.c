#include <malloc.h>
#include "mbr.h"

struct MasterBootRecord* mbr_load(const char* diskPath) {
    FILE* disk = fopen(diskPath, "rb");
    if (disk == NULL) {
        return NULL;
    }

    struct MasterBootRecord* mbr = malloc(MBR_SIZE);
    size_t read = fread(mbr, MBR_SIZE, 1, disk);
    if (read != 1) {
        fclose(disk);
        return NULL;
    }
    fclose(disk);
    return mbr;
}

char* mbr_decode_partition_type(const struct PartitionEntry* partition) {
    switch (partition->type) {
        case 0x00:
            return "Empty";
        case 0x0E:
            return "FAT16";
        default:
            return "Unknown";
    }
}

bool mbr_is_partition_bootable(const struct PartitionEntry* partition) {
    return partition->bootFlag == MBR_BOOT_FLAG;
}

bool mbr_is_valid_boot_signature(uint16_t bootSignature) {
    return bootSignature == MBR_BOOT_SIGNATURE;
}