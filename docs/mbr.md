#  Master Boot Record partition table

The Master Boot Record (MBR) is a legacy format that defines partitions on a disk. The MBR has been replaced by the GUID Partition Table (GPT).

This format is not standardized. Its various uses actually serve as the standards. Furthermore, there are multiple variations depending on manufacturers and time periods.

## Understanding MBR

When the BIOS starts, it loads the first sector of the disk into memory at address `0x7C00`, and then executes the code at that location. This first sector is called the "Master Boot Record" and has a size of 512 bytes.

The code executed by the BIOS is called the bootloader. Its main task is to search for an active partition and boot the operating system cont

There are multiple ways of organizing the MBR. `parted` and `fdisk` appear to use the "Structure of a modern standard MBR" defined on the [Master boot record Wikipedia page](https://en.wikipedia.org/wiki/Master_boot_record#Sector_layout).
ained within that partition.

### MBR layout

Because there are many of optional values, the following table is only displays the values used by the `parted` or `fdisk` tools.

| Offset  | Size | Description                         |
|---------|------|-------------------------------------|
|  0x0000 |  218 | Bootstrap code area (part 1)        |
|  0x00DA |    6 | unsed by either `parted` or `fdisk` |
|  0x00E0 |  216 | Bootstrap code area (part 2)        |
|  0x01B8 |    4 | Disk signature                      |
|  0x01BC |    2 | unsed by either `parted` or `fdisk` |
|  0x01BE |   16 | Primary partition entry #1          |
|  0x01CE |   16 | Primary partition entry #2          |
|  0x01DE |   16 | Primary partition entry #3          |
|  0x01EE |   16 | Primary partition entry #4          |
|  0x01FE |    2 | Boot signature                      |
| Size    |  512 |                                     |

The bootloader is divided into two parts.
The first part is located at address `0x00` with a size of 218 bytes, and the second part is at address `0x00E0` with a size of 216 bytes.
Thus, during execution, the code is located respectively at addresses `0x7C00` and `0x7CE0` (`0x7C00` + `0x00E0`).

The boot signature is always `0xAA55`. It is used by the BIOS to identify that the sector contains a bootloader.

Each primary partition entry has the following format: (the offset is relative to the start of the corresponding partition entry).


| Offset  | Size | Description                          |
|---------|------|--------------------------------------|
|    0x00 |    1 | Partition flags                      |
|    0x01 |    3 | CHS Address of partition start       |
|    0x04 |    1 | Partition type                       |
|    0x05 |    3 | CHS address of last partition sector |
|    0x08 |    4 | LBA of partition start               |
|    0x0C |    4 | Number of sectors in partition       |
| Size    |   16 |                                      |

In modern storage devices, the concept of cylinders and heads is no longer relevant, and CHS (Cylinder-Head-Sector) addressing is not accurate or reliable. Instead, LBA (Logical Block Addressing) is the standard method used to address sectors on disks. The LBA of the partition end can be calculated by adding the number of sectors in the partition to the LBA of the partition start.

Only the first bit is used in the flags. If it is set, it indicates that the partition is active, meaning it is bootable.

The list of partition types can be found on the Wikipedia page [Partition type](https://en.wikipedia.org/wiki/Partition_type#List_of_partition_IDs).

Here are some values for well-known partition types:
| Type  | Value |
|-------|-------|
| FAT16 |  0x0e |
| FAT32 |  0x0c |


## Analyse of partition tables

In the examples below, we create an MBR partition table with two partitions, each with a size of 10MiB. The boot flag is enabled for the first partition. The first partition is formatted as FAT16, and the second partition is formatted as FAT32.

### With parted

```bash
$ dd if=/dev/zero of=disk.bin bs=1M count=128
$ parted -s disk.bin mktable msdos
$ parted -s disk.bin mkpart primary fat16 1MiB 11MiB
$ parted -s disk.bin set 1 boot on
$ parted -s disk.bin mkpart primary fat32 11MiB 12MiB
$ parted -s disk.bin unit s print
Model:  (file)
Disk disk.bin: 262144s
Sector size (logical/physical): 512B/512B
Partition Table: msdos
Disk Flags: 

Number  Start   End     Size    Type     File system  Flags
 1      2048s   22527s  20480s  primary               boot, lba
 2      22528s  43007s  20480s  primary               lba
```

The content of the disk is:
```
$ hexdump -C disk.bin
00000000  fa b8 00 10 8e d0 bc 00  b0 b8 00 00 8e d8 8e c0  |................|
00000010  fb be 00 7c bf 00 06 b9  00 02 f3 a4 ea 21 06 00  |...|.........!..|
00000020  00 be be 07 38 04 75 0b  83 c6 10 81 fe fe 07 75  |....8.u........u|
00000030  f3 eb 16 b4 02 b0 01 bb  00 7c b2 80 8a 74 01 8b  |.........|...t..|
00000040  4c 02 cd 13 ea 00 7c 00  00 eb fe 00 00 00 00 00  |L.....|.........|
00000050  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000001b0  00 00 00 00 00 00 00 00  21 1e 79 a3 00 00 80 00  |........!.y.....|
000001c0  01 10 0e 03 20 af 00 08  00 00 00 50 00 00 00 00  |.... ......P....|
000001d0  01 b0 0c 03 60 4f 00 58  00 00 00 50 00 00 00 00  |....`O.X...P....|
000001e0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000001f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 55 aa  |..............U.|
00000200  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
08000000
```

The hexadecimal values are written in little-endian notation. They should be read from right to left.

This translates to a more human-readable table as follows:
| Offset | Description                         | Hex value   | Comments               |
|--------|-------------------------------------|-------------|------------------------|
| 0x0000 | Bootloader                          |             | default bootloader     |
| 0x004B | unsed                               | 00...       |                        |
| 0x01B8 | Disk signature                      | a3 79 1e 21 |                        |
| 0x01BC | unsed                               |                                      |
| 0x01BE | Partition #1 flags                  | 80          | Boot flag set          |
| 0x01BF | Partition #1 CHS start              | 10 01 00    |                        |
| 0x01C2 | Partition #1 type                   | 0e          | FAT16B with LBA        |
| 0x01C3 | Partition #1 CHS end                | af 20 03    |                        |
| 0x01C7 | Partition #1 LBA start              | 00 00 08 00 | 2048th sector          |
| 0x01CA | Partition #1 Number of sectors      | 00 00 50 00 | 20,480 sectors         |
| 0x01CE | Partition #2 flags                  | 00          | Boot flag unset        |
| 0x01CF | Partition #2 CHS start              | b0 01 00    |                        |
| 0x01D2 | Partition #2 type                   | 0c          | FAT32B with LBA        |
| 0x01D3 | Partition #2 CHS end                | 4f 60 03    |                        |
| 0x01D6 | Partition #2 LBA start              | 00 00 58 00 | 22,528th sector        |
| 0x01DA | Partition #2 Number of sectors      | 00 00 50 00 | 20,480 sectors         |
| 0x01DE | undefined partition #3              | 00...       |                        |
| 0x01EE | undefined partition #4              | 00...       |                        |
| 0x01FE | Boot signature                      | aa 55       |                        |
| 0x0200 | empty space                         | 00...       |                        |

Until address `0x4A`, parted defines its own bootloader. See the page [Understanding the bootloader in parted](https://github.com/gronono/trainos/blob/main/docs/parted-bootloader.md).

The LBA of the first partition's start is `0x800`. This corresponds to the expected value of 2048 sectors that you passed to `parted` when creating the partition.

The number of sectors in the first partition is `0x5000`. Since the sector size is 512 bytes, the partition has the expected size of 10 MiB.

The second partition starts just after the first one, at the 22,528th sector. This value is obtained by adding the starting sector of the first partition (2,048th) to the number of sectors in the first partition (20,480).

### With fdisk

```bash
$ dd if=/dev/zero of=disk.bin bs=1M count=128
$ fdisk disk.bin
Welcome to fdisk (util-linux 2.37.2).
Changes will remain in memory only, until you decide to write them.
Be careful before using the write command.

Device does not contain a recognized partition table.
Created a new DOS disklabel with disk identifier 0xfd1742e6.

Command (m for help): n
Partition type
   p   primary (0 primary, 0 extended, 4 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (1-4, default 1): 
First sector (2048-262143, default 2048): 
Last sector, +/-sectors or +/-size{K,M,G,T,P} (2048-262143, default 262143): +10M

Created a new partition 1 of type 'Linux' and of size 10 MiB.
Command (m for help): t
Selected partition 1
Hex code or alias (type L to list all): 0e
Changed type of partition 'Linux' to 'W95 FAT16 (LBA)'.

Command (m for help): a
Partition number (1,2, default 2): 1

The bootable flag on partition 1 is enabled now.

Command (m for help): n
Partition type
   p   primary (1 primary, 0 extended, 3 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (2-4, default 2): 
First sector (22528-262143, default 22528):  
Last sector, +/-sectors or +/-size{K,M,G,T,P} (22528-262143, default 262143): +10M 

Created a new partition 2 of type 'Linux' and of size 10 MiB.

Command (m for help): n
Partition type
   p   primary (1 primary, 0 extended, 3 free)
   e   extended (container for logical partitions)
Select (default p): p
Partition number (2-4, default 2): 
First sector (22528-262143, default 22528):  
Last sector, +/-sectors or +/-size{K,M,G,T,P} (22528-262143, default 262143): +10M 

Created a new partition 2 of type 'Linux' and of size 10 MiB.

Command (m for help): t
Partition number (1,2, default 2): 2
Hex code or alias (type L to list all): 0c

Changed type of partition 'Linux' to 'W95 FAT32 (LBA)'.

Command (m for help): w
The partition table has been altered.
Syncing disks.

$ fdisk -l disk.bin
Disk disk.bin: 128 MiB, 134217728 bytes, 262144 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x6d620daa

Device     Boot Start   End Sectors Size Id Type
disk.bin1  *     2048 22527   20480  10M  e W95 FAT16 (LBA)
disk.bin2       22528 43007   20480  10M  c W95 FAT32 (LBA)
```

The content of the disk is:
```
$ hexdump -C disk.bin
00000000  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
000001b0  00 00 00 00 00 00 00 00  aa 0d 62 6d 00 00 80 20  |..........bm... |
000001c0  21 00 0e 66 25 01 00 08  00 00 00 50 00 00 00 66  |!..f%......P...f|
000001d0  26 01 0c ac 2a 02 00 58  00 00 00 50 00 00 00 00  |&...*..X...P....|
000001e0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
000001f0  00 00 00 00 00 00 00 00  00 00 00 00 00 00 55 aa  |..............U.|
00000200  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
*
08000000
```

The hexadecimal values are written in little-endian notation. They should be read from right to left.

This translates to a more human-readable table as follows:
| Offset | Description                         | Hex value   | Comments               |
|--------|-------------------------------------|-------------|------------------------|
| 0x0000 | unsed                               | 00...       |                        |
| 0x01B8 | Disk signature                      | 6d 62 0d aa |                        |
| 0x01BC | unsed                               | 00...                                |
| 0x01BE | Partition #1 flags                  | 80          | Boot flag set          |
| 0x01BF | Partition #1 CHS start              | 00 21 20    |                        |
| 0x01C2 | Partition #1 type                   | 0e          | FAT16B with LBA        |
| 0x01C3 | Partition #1 CHS end                | 01 25 01    |                        |
| 0x01C7 | Partition #1 LBA start              | 00 00 08 00 | 2048th sector          |
| 0x01CA | Partition #1 Number of sectors      | 00 00 50 00 | 20,480 sectors         |
| 0x01CE | Partition #2 flags                  | 00          | Boot flag unset        |
| 0x01CF | Partition #2 CHS start              | 01 26 66    |                        |
| 0x01D2 | Partition #2 type                   | 0c          | FAT32B with LBA        |
| 0x01D3 | Partition #2 CHS end                | 02 2A AC    |                        |
| 0x01D6 | Partition #2 LBA start              | 00 00 58 00 | 22,528th sector;       |
| 0x01DA | Partition #2 Number of sectors      | 00 00 50 00 | 20,480 sectors         |
| 0x01DE | undefined partition #3              | 00...       |                        |
| 0x01EE | undefined partition #4              | 00...       |                        |
| 0x01FE | Boot signature                      | aa 55       |                        |
| 0x0200 | empty space                         | 00...       |                        |

Unlike `parted`, `fdisk` does not provide a bootloader.

CHS addresses are different between parted and fdisk. As previously mentioned, they are not reliable.
But LBA addresses remain consistent and unchanged.
