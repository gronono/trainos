# Disk 
 
## Layout 
 
| Offset   | Size | Value       | Description                          |
|----------|------|-------------|--------------------------------------|
| 0x000000 |  440 |  stage1.asm | MBR                                  |
| 0x0001B8 |    4 |             | Disk signature                       |
| 0x0001BC |    2 |             | unsed                                |
| 0x0001BE |    1 |          80 | 0xPartition #1 flags                 |
| 0x0001BF |    3 |             | Partition #1 CHS start (unsed)       |
| 0x0001C2 |    1 |          0E | Partition #1 type                    |
| 0x0001C3 |    3 |             | Partition #1 CHS end (unsed)         |
| 0x0001C6 |    4 | 00 00 08 00 | Partition #1 Number of sectors       |
| 0x0001CE |   16 |             | Primary partition entry #2 (unsed)   |
| 0x0001DE |   16 |             | Primary partition entry #3 (unsed)   |
| 0x0001EE |   16 |             | Primary partition entry #4 (unsed)   |
| 0x0001FE |    2 |       AA 55 | Boot signature                       |
| 0x000200 |      |             | unused                               |
| Partition #1 (FAT 16)                                                |
| 0x100000 |   62 |             | FAT16 Headers                        |
| 0x10003E |  448 |      stage1 | Stage 1 boot code                    |
| 0x1001FE |    2 |       AA 55 | Boot signature                       |
