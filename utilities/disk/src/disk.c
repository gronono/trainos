#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "mbr.h"

char* format_size(uint64_t bytes) {
    const char* suffixes[] = {"B", "KB", "MB", "GB", "TB"};
    int suffix_index = 0;
    double size = (double) bytes;
    char* formatted_size = malloc(20);

    while (size >= 1024 && suffix_index < 4) {
        size /= 1024;
        suffix_index++;
    }

    sprintf(formatted_size, "%.2f %s", size, suffixes[suffix_index]);

    return formatted_size;
}

void print_bootstrap(const struct MasterBootRecord* mbr) {
    for (uint32_t i = 0; i < sizeof(mbr->bootstrap); i += 16) {
        printf("%04X: ", i);
        for (int j = 0; j < 16; ++j) {
            if (i + j < 440) {
                printf("%02X ", mbr->bootstrap[i + j]);
            } else {
                // in case we overtaken bootstrap size
                printf("   ");
            }
        }
        printf(" | ");
        for (int j = 0; j < 16; ++j) {
            if (i + j < 440) {
                uint8_t c = mbr->bootstrap[i + j];
                printf("%c", (c >= 32 && c <= 126) ? c : '.');
            } else {
                // to align with character format
                printf(" ");
            }
        }
        printf("\n");
    }
}

void print_partition(const struct PartitionEntry* partition) {
    printf("    Boot Flag: 0x%02X %s\n", partition->bootFlag, mbr_is_partition_bootable(partition) ? "(bootable)" : "");
    printf("    CHS Start: 0x%02X 0x%02X 0x%02X\n",
           partition->chsStart.cylinder, partition->chsStart.head, partition->chsStart.sector);
    printf("    Type: 0x%02X (%s)\n", partition->type, mbr_decode_partition_type(partition));
    printf("    CHS Last Sector: 0x%02X 0x%02X 0x%02X\n",
           partition->chsLastSector.cylinder, partition->chsLastSector.head, partition->chsLastSector.sector);
    printf("    LBA Start: 0x%08X\n", partition->lbaStart);
    printf("    Number of Sectors: 0x%08X (%s)\n", partition->numberSectors,
           format_size(partition->numberSectors * MBR_SECTOR_SIZE));
}

void print_mbr(struct MasterBootRecord* mbr) {
    if (!mbr_is_valid_boot_signature(mbr->bootSignature)) {
        fprintf(stderr, "Invalid boot signature\n");
        return;
    }

    printf("Bootstrap:\n");
    print_bootstrap(mbr);
    printf("\n");

    printf("Disk ID: 0x%08X\n", mbr->diskId);
    printf("Reserved: 0x%04X\n", mbr->reserved);
    printf("Partitions:\n");
    for (int i = 0; i < 4; ++i) {
        printf("  Partition %d:\n", i);
        print_partition(&mbr->partitions[i]);
    }

    printf("Boot Signature: 0x%04X\n", mbr->bootSignature);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: boot <disk>\n");
        return 1;
    }

    char* diskPath = argv[1];
    struct MasterBootRecord* mbr = mbr_load(diskPath);
    if (mbr == NULL) {
        fprintf(stderr, "Failed to load MBR on %s\n", diskPath);
        return 2;
    }

    print_mbr(mbr);

    free(mbr);
}

