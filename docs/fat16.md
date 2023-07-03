Example:
```bash
dd if=/dev/zero of="${DISK}" bs=1M count=64
parted -s "${DISK}" mkpart primary fat16 2048s 22528s
parted -s "${DISK}" set 1 boot on
sudo losetup -Pf --show "${DISK}"
sudo mkfs.fat -F 16 -n TRAINOS -f 1 /dev/loop10p1

```

| Offset | Size | Value       | Description                                      |
|--------|------|-------------|--------------------------------------------------|
|  0x000 |    3 |    EB 3C 90 | Jump to skip headers                             |
|  0x003 |    8 |    mkfs.fat | OEM identifier                                   |
|  0x00B |    2 |       00 02 | The number of Bytes per sector (512 bytes)       |
|  0x00D |    1 |          04 | Number of sectors per cluster                    |
|  0x00E |    2 |       04 00 | Number of reserved sectors (4 sectors)           | 
|  0x010 |	  1	|          01 | Number of File Allocation Tables                 |
|  0x011 |    2	|       00 02 | Number of root directory entries (512)           |
|  0x013 |    2	|       00 50 | The total sectors in the logical volume (20480s) |
|  0x015 |    1	|          F8 | Media descriptor type                            |
|  0x016 |    2	|       14 00 | Number of sectors per FAT                        |
|  0x018 |    2	|       20 00 | Number of sectors per track                      |
|  0x01A |    2	|       08 00 | Number of heads (8 heads)                        |
|  0x01C |    4	| 00 08 00 00 | Number of hidden sectors (2048s)                 |
|  0x020 |    4	| 00 00 00 00 | Large sector count                               |
|  0x024 |    1	|          80 | Drive number                                     |
|  0x025 |    1	|          00 | Reserved                                         |
|  0x026 |    1	|          29 | Signature (must be 0x28 or 0x29)                 |
|  0x027 |    4	| B8 CD 6B BE | VolumeID 'Serial' number                         |
|  0x02B |   11 |     TRAINOS | Volume label string                              |
|  0x036 |    8	|       FAT16 | System identifier string                         |
|  0x03E |  448 |  stage1.asm |	Boot code                                        | 
|  0x01F |    2 | 	    55 AA | Bootable partition signature 0xAA55              |

reserved sectors = 0x40 = 4
fat sectors = number of FAT x sectors per FAT = 0x01 x 0x0014 = 0x14 = 20
root directory start = reserved sectors + fat sectors = 24
root directory length = (number of root directory entries x 32 + bytes per sector - 1) / bytes per sector = (512 * 32 + 512 - 1) / 512 = 32 sectors
root directory end = root directory start + length = 56 sectors

Partition start at sector 2048 so at address 0x100000. So root directory start at 0x103000.

