# Disk

## Layout

| Offset                | Size | Value       | Description                         |
|-----------------------|------|-------------|-------------------------------------|
| 0x000000              | 440  | mbr.asm     | MBR                                 |
| 0x0001B8              | 4    |             | Disk signature                      |
| 0x0001BC              | 2    |             | unsed                               |
| 0x0001BE              | 1    | 80          | 0xPartition #1 flags                |
| 0x0001BF              | 3    |             | Partition #1 CHS start (unsed)      |
| 0x0001C2              | 1    | 0E          | Partition #1 type                   |
| 0x0001C3              | 3    |             | Partition #1 CHS end (unsed)        |
| 0x0001C6              | 4    | 00 00 08 00 | Partition #1 LBA                    |
| 0x0001CA              | 4    | 00 00 F8 00 | Partition #1 Number of sectors      |
| 0x0001CE              | 16   |             | Primary partition entry #2 (unsed)  |
| 0x0001DE              | 16   |             | Primary partition entry #3 (unsed)  |
| 0x0001EE              | 16   |             | Primary partition entry #4 (unsed)  |
| 0x0001FE              | 2    | AA 55       | Boot signature                      |
| 0x000200              |      |             | unused                              |
| Partition #1 (FAT 16) |
| 0x100000              | 3    | EB 3C 90    | Jump to skip headers                |
| 0x100003              | 8    | mkfs.fat    | OEM identifier                      |
| 0x10000B              | 2    | 02 00       | Bytes per sector (512 bytes)        |
| 0x10000D              | 1    | 04          | Number of sectors per cluster       |
| 0x10000E              | 2    | 00 04       | Number of reserved sectors          |
| 0x100010              | 1    | 01          | Number of File Allocation Tables    |
| 0x100011              | 2    | 02 00       | Number of root directory entries    |
| 0x100013              | 2    | F8 00       | Total sectors in the logical volume |
| 0x100015              | 1    | F8          | Media descriptor type               |
| 0x100016              | 2    | 00 40       | Number of sectors per FAT           |
| 0x100018              | 2    | 00 20       | Number of sectors per track         |
| 0x10001A              | 2    | 00 04       | Number of heads or sides            |
| 0x10001C              | 4    | 00 00 08 00 | Number of hidden sectors            |
| 0x100020              | 4    | 00 00 00 00 | Large sector count                  |
| 0x100024              | 1    | 80          | Drive number                        |
| 0x100025              | 1    | 00          | Reserved                            |
| 0x100026              | 1    | 29          | Signature                           |
| 0x100027              | 4    |             | VolumeID 'Serial' number            |
| 0x10002B              | 11   | TRAINOS     | Volume label string                 |
| 0x100036              | 8    | FAT16       | System identifier string            |
| 0x10003E              | 450  | vbr         | Volume Boot Record code             |
